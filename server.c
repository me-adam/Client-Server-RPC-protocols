/*original code from:
https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/
modified to run on uxb4.wiu.edu
*/
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#define MAX 80 
#define PORT 8080
  // this method convert the the numerical numbers to named number and return the result 
char * convert(int n){
    int l=0;
    int y = 0;
    int lastzero;
    //char finalString[200];
    char forzerozs[200];
    bzero(forzerozs,sizeof(forzerozs));
    //alocate memory to the return value
    char *finalString = malloc (sizeof (4999) * MAX);
    bzero(finalString,sizeof(char));
    //check if the number negative to concate negative to the result in the first
    if(n<0){
        n=n*(-1);
        strcat(finalString,"negative ");
    }
    //check if the number is zero put zero and return that
    if(n==0){
      strcat(finalString,"zero ");
    }else{
/* here will check if the last of the number if 0 it will put zero by that number of 0, because we will reverse the number for     better starting from the left using %10 operation */
        lastzero=n%10;
        while(lastzero==0){
            strcat(forzerozs,"zero ");
            n = n/10;
            lastzero=n%10;
        }
        /* here reverse the number to make it easy fo me to just use %10 operation to convert the numerical number to named number */
        while (n != 0)
        {
            l = l*10;
            l = l+n%10;    
            n = n/10;
        }
    
        while(l!=0){
            y=l%10;
            l=l/10;
            switch(y){// witch to convert the number after cut them by using %10
                case 0:strcat(finalString,"zero ");break;
                case 1:strcat(finalString,"one ");break;
                case 2:strcat(finalString,"two ");break;
                case 3:strcat(finalString,"three ");break;
                case 4:strcat(finalString,"four ");break;
                case 5:strcat(finalString,"five ");break;
                case 6:strcat(finalString,"six ");break;
                case 7:strcat(finalString,"seven ");break;
                case 8:strcat(finalString,"eight ");break;
                case 9:strcat(finalString,"nine ");break;
                default:break;
            }//end of switch
        }// end of while loop
        //concate the last zeros if there was any
        strcat(finalString,forzerozs);
    }//end of else the number is not zero
    return finalString;
}//end of convert method
// Function designed for chat between client and server. 
void func(int sockfd) 
{ 
    char buff[MAX]; 
    /* x,y,x will be use to calculate the input, put the result in z */
    int x;
    int y;
    int z;
    //int n; 
    // infinite loop for chat 
    for (;;) { 
        bzero(buff, MAX); 
  
        // read the message from client and copy it in buffer 
        read(sockfd, buff, sizeof(buff)); 

        // if msg contains "Exit" then server exit and chat ended. 
        if (strncmp("exit", buff, 4) == 0) { 
            write(sockfd, buff, sizeof(buff)); 
            printf("Server Exit...\n"); 
            break; 
        } 

        /* use token to get the operation and the arguments */
        char* token=strtok(buff," ");
        /* start by checkig the operation then extract the two arguments and performe the operation and print the client operation*/
        if(strcmp(token,"add")==0)
        {
            x=atoi(strtok(NULL," "));
            y=atoi(strtok(NULL," "));
            z=x+y;
            printf("From client: add %d %d\n",x,y);

        }
        else if(strcmp(token,"sub")==0){
            x=atoi(strtok(NULL," "));
            y=atoi(strtok(NULL," "));
            z=x-y;
            printf("From client: sub %d %d\n",x,y);
        }else if(strcmp(token,"mul")==0){
            x=atoi(strtok(NULL," "));
            y=atoi(strtok(NULL," "));
            z=x*y;
            printf("From client: mul %d %d\n",x,y);

       }else if(strcmp(token,"div")==0){
            x=atoi(strtok(NULL," "));
            y=atoi(strtok(NULL," "));
            //check for zero division
            if(y==0){
                char notS[80]="zero division error";
                printf("From client div: %d %d\n",x,y);
                write(sockfd, notS, sizeof(notS)); 
                continue;
            }
            z=x/y;
            printf("From client: div %d %d\n",x,y);

       }else{// if none of the above was found will sned not supported operation and continue the loop */
           char notS[80]="not supported";
            x=atoi(strtok(NULL," "));
            y=atoi(strtok(NULL," "));
            printf("From client: %s %d %d\n",token,x,y);
            write(sockfd, notS, sizeof(notS)); 
            continue;
       }//end of else
        bzero(buff, MAX); 
        //after performe the operation call convert() method to converte the result to named number
        char* re=convert(z);
        sprintf(buff,"%s",re);
         // and send that buffer to client 
        write(sockfd, buff, sizeof(buff)); 
  

    }// end of for loop 
}//end of func method
  
// Driver function 
int main() 
{ 
    int sockfd, connfd, len; 
    struct sockaddr_in servaddr, cli; 
  
    // socket create and verification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("socket creation failed...: %s\n",strerror(errno)); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n"); 
    bzero(&servaddr, sizeof(servaddr)); 
  
    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
  
    // Binding newly created socket to given IP and verification 
    if ((bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) { 
        printf("socket bind failed...: %s\n", strerror(errno)); 
        exit(0); 
    } 
    else
        printf("Socket successfully binded..\n"); 
  
    // Now server is ready to listen and verification 
    if ((listen(sockfd, 5)) != 0) { 
        printf("Listen failed...: %s\n",strerror(errno)); 
        exit(0); 
    } 
    else
        printf("Server listening..\n"); 
    len = sizeof(cli); 
  
    // Accept the data packet from client and verification 
    connfd = accept(sockfd, (struct sockaddr*)&cli, (socklen_t *)&len); 
    if (connfd < 0) { 
        printf("server acccept failed...: %s\n",strerror(errno)); 
        exit(0); 
    } 
    else
        printf("server acccept the client...\n"); 
  
    // Function for chatting between client and server 
    func(connfd); 
  
    // After chatting close the socket 
    close(sockfd); 

    return 0;

	
} 
