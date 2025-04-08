#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <arpa/inet.h>
#include <time.h>
#define PORT 8080
#define BUFFER_SIZE 1024
#define WINDOW_SIZE 4
#define SERVER_IP "127.0.0.1"
#define TOTAL_PACKET 10
#define TIMEOUT_SEC 2

int main(){
    int sockfd;
    struct sockaddr_in server_addr;
    socklen_t addr_len=sizeof(server_addr);
    char buffer[BUFFER_SIZE];
    int base=0, next_seq=0;
    int ack_received[TOTAL_PACKET]={0};

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if(inet_pton(AF_INET,SERVER_IP,&server_addr.sin_addr)<0){
        perror("Invalid server address");
    }
    printf("Sender started. Sending %d packets...\n\n", TOTAL_PACKET);
    while(base<TOTAL_PACKET){
        while(next_seq<base+WINDOW_SIZE && next_seq<TOTAL_PACKET){
            sprintf(buffer,"Packet:%d",next_seq);
            sendto(sockfd,buffer,strlen(buffer),0,(struct sockaddr*)&server_addr,addr_len);
            printf("Sent:%s\n",buffer);
            next_seq++;
        }
        struct timeval timeout;
        timeout.tv_sec=TIMEOUT_SEC;
        timeout.tv_usec=0;
        setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,&timeout,sizeof(timeout));

        while(1){
            int n=recvfrom(sockfd,buffer,BUFFER_SIZE,0,(struct sockaddr*)&server_addr,&addr_len);
            if(n<0){
                printf("timeout,!retransimitting from packet %d\n",base);
                next_seq=base;
                break;
            }
            buffer[n]='\0';
            int ack_num=atoi(buffer+4);
            printf("Received ACK%d\n",ack_num);
            ack_received[ack_num]=1;
            while(base<TOTAL_PACKET && ack_received[base]){
                base++;
            }
            if (base==TOTAL_PACKET){
                printf("all the packets are transmitted\n");
                break;
            }
        }
    }
    close(sockfd);
    return 0;

}
