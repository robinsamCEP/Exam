#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>


#define PORT 8080
#define BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"
int main(){
    int sock=0;
    struct sockaddr_in server_addr;
    int addrlen=sizeof(server_addr);
    char buffer[BUFFER_SIZE]={0};
    char *message="Hello from Client";

    if((sock=socket(AF_INET,SOCK_STREAM,0))<0){
        perror("Socket creation error");
        return -1;
    }
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(PORT);
    if(inet_pton(AF_INET,SERVER_IP,&server_addr.sin_addr)<0){
        perror("Invalid address or Address not supported");
        return -1;
    }
    if((connect(sock,(struct sockaddr*)&server_addr,sizeof(server_addr)))<0){
        perror("Connection failed");
        return -1;
    }
    send(sock, message, strlen(message), 0);
    printf("Message sent to server.\n");

   
    read(sock, buffer, BUFFER_SIZE);
    printf("Server: %s\n", buffer);

    
    close(sock);
    return 0;
}