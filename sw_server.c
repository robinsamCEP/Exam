#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>                        
#include <arpa/inet.h> 

#define PORT 8080
#define BUFFER_SIZE 1024

int handle_client(int sockfd){
    char buffer[BUFFER_SIZE];
    int packet_count=0;
    while(1){
        ssize_t n=recv(sockfd,buffer,BUFFER_SIZE,0);
        if(n<0){
            break;
        }
        buffer[n]='\0';
        printf("Recived:%s\n", buffer);
        send(sockfd, "ACK", 3, 0);
        printf("sent acknowledgment for packet%d\n", ++packet_count);
    }
}
int main(){
    int sockfd, newsockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd, 5) < 0) {
        perror("listen failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("server is listening on port%d...\n", PORT);
    if ((newsockfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_len)) < 0) {
        perror("accept failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    handle_client(newsockfd);
    close(newsockfd);
    close(sockfd);
    return 0;
}
