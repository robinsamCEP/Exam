#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <arpa/inet.h> 
#include <time.h> 

#define PORT 8080
#define BUFFER_SIZE 1024
#define LOSS_PROBABILITY 20

int main(){
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len=sizeof(client_addr);
    char buffer[BUFFER_SIZE];
    int expected_seq=0;

    if((sockfd=socket(AF_INET,SOCK_DGRAM,0))<0){
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=INADDR_ANY;
    server_addr.sin_port=htons(PORT);
    if((bind(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr)))<0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    srand(time(0));
    while(1){
        int n=recvfrom(sockfd,buffer,BUFFER_SIZE,0,(struct sockaddr*)&client_addr,&addr_len);
        if ( n<0){
            printf("Recieve failed");
            break;
        }
        buffer[n]='\0';
        int seq_num=atoi(buffer+7);
        if((rand()%100)<LOSS_PROBABILITY){
            printf("Packet %d lost (simulated)\n",seq_num);
            continue;
        }
        if(seq_num==expected_seq){
            printf("Recieved Packet %d, sending ACK %d\n",seq_num,seq_num);
            sprintf(buffer,"ACK:%d",seq_num);
            sendto(sockfd,buffer,strlen(buffer),0,(struct sockaddr*)&client_addr,addr_len);
            expected_seq++;
        
        }
        else{
            printf("out-of-order,packet %d ,expected %d, ignored\n",seq_num,expected_seq);
        }
    }
    close(sockfd);
    return 0;
}
