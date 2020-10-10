/*original code from:
https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/
modified to run on uxb4.wiu.edu
*/
//@Mustafa Adam
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#define MAX 80 
#define PORT 8080 

void func(int sockfd) 
{ 
    char buff[MAX]; 
    int n;
    char *last; 

    char secbuff[MAX];
    for (;;) { 
        int count=1;
        bzero(buff, sizeof(buff)); 
        printf("Enter the request : "); 
        n = 0;
        while ((buff[n++] = getchar()) != '\n');   
        strcpy(secbuff,buff);
        last=strtok(secbuff," ");
        
        while(strtok(NULL," ")!=NULL){
            count++;
        }
        if(count!=3&&(strncmp(buff, "exit", 4)) != 0){
            printf("%s only supports 2 arguments!;\n",last);
            continue;
        }
        write(sockfd, buff, sizeof(buff)); 
        bzero(buff, sizeof(buff)); 
        read(sockfd, buff, sizeof(buff)); 
        printf("From Server : %s\n", buff); 
        if ((strncmp(buff, "exit", 4)) == 0) { 
            printf("Client Exit...\n"); 
            break; 
        } 
    } 
} 
  
int main() 
{ 
    int sockfd; 
    struct sockaddr_in servaddr; 
  
    // socket create and varification 
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
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    servaddr.sin_port = htons(PORT); 
  
    // connect the client socket to server socket 
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) { 
        printf("connection with the server failed...: %s\n",strerror(errno)); 
        exit(0); 
    } 
    else
        printf("connected to the server..\n"); 
  
    // function for chat 
    func(sockfd); 
  
    // close the socket 
    close(sockfd);

    return 0;
 
} 
