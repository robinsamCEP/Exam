#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(){
    int server_fd,new_socket;
    struct sockaddr_in address;
    int addr_len=sizeof(address);
    char buffer[BUFFER_SIZE]={0};
    char *message= "Hello from Server";

    if((server_fd=socket(AF_INET,SOCK_STREAM,0))<0){
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family=AF_INET;
    address.sin_addr.s_addr=INADDR_ANY;
    address.sin_port=htons(PORT);

    if((bind(server_fd,(struct sockaddr*)&address,sizeof(address)))<0){
        perror("bind creation failed");
        exit(EXIT_FAILURE);
    }
    printf("Server is Listening on the port %d\n",PORT);
    if((listen(server_fd,3))<0){
        perror("Listening Failed");
        exit(EXIT_FAILURE);
    }
    if((new_socket=accept(server_fd,(struct sockaddr*)&address,(socklen_t*)&addr_len))<0){
        perror("accept failed");
        exit(EXIT_FAILURE);

    }
    read(new_socket,buffer,BUFFER_SIZE);
    printf("Received message:%s\n",buffer);

    send(new_socket,message,strlen(message),0);
    printf("message sent to Client");
    
    close(new_socket);
    close(server_fd);
   
}