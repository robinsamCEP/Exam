#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h> 

#define PORT 8080
#define BUFFER_SIZE 1024

int send_data(int sockfd,char *data,int total_packets,int timer_interval_ms){
    char buffer[BUFFER_SIZE];
    for( int i =0;i< total_packets;i++){
        snprintf(buffer,sizeof(buffer),"packet%d",i+1);
        printf("sending packet %d",i+1);
        usleep(timer_interval_ms*1000);
        send(sockfd,buffer,strlen(buffer),0);
        printf("sent packet %d\n",i+1);
        recv(sockfd, buffer, sizeof(buffer), 0);
        printf("Received acknowledgment for packet %d\n", i + 1);
    }
}
int main(){
    int sockfd;
    struct sockaddr_in server_addr;
    char data[BUFFER_SIZE * 10] = "this is a stop-and-wait protocol example.";

    int total_packets = 10;
    int timer_interval_ms = 500;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    send_data(sockfd, data, total_packets, timer_interval_ms);
    close(sockfd);
    return 0;
}
