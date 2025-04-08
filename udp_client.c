#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>


#define PORT 8080
#define BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"

int main(){
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(server_addr);

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(PORT);
    if(inet_pton(AF_INET,SERVER_IP,&server_addr.sin_addr)<0){
        printf("invalid address");
        exit(EXIT_FAILURE);
    }
    while(1){
        printf("Enter the message that sent to Server:");
        fgets(buffer,BUFFER_SIZE,stdin);
        buffer[strcspn(buffer,"\n")]='\0';
        sendto(sockfd,buffer,strlen(buffer),0,(struct sockaddr*)&server_addr,addr_len);
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server_addr, &addr_len);
        buffer[n] = '\0';
        printf("Server: %s\n", buffer);
    }
    close(sockfd);
    return 0;
}
