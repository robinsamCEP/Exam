 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <arpa/inet.h> 
#include <time.h> 
#define PORT 8080 
#define SERVER_IP "127.0.0.1" 
#define WINDOW_SIZE 4 
#define TOTAL_PACKETS 10 
#define TIMEOUT 2  
 
typedef struct { 
    int seq_no; 
    char data[1024]; 
} packet; 
int main() { 
    int sockfd; 
    struct sockaddr_in server_addr; 
    socklen_t addr_size; 
    packet pkt, ack_pkt; 
    int base = 0, next_seq = 0; 
    fd_set readfds; 
    struct timeval timeout; 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd < 0) { 
        perror("Socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
    server_addr.sin_family = AF_INET; 
    server_addr.sin_port = htons(PORT); 
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP); 
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) { 
        perror("Connection failed"); 
        close(sockfd); 
        exit(EXIT_FAILURE); 
    } 
    while (base < TOTAL_PACKETS) { 
        while (next_seq < base + WINDOW_SIZE && next_seq < TOTAL_PACKETS) { 
            pkt.seq_no = next_seq; 
            sprintf(pkt.data, "Hello %d", next_seq); 
            send(sockfd, &pkt, sizeof(pkt), 0); 
            printf("Sent packet: %d\n", pkt.seq_no); 
            next_seq++; 
        } 
        while (base < next_seq) { 
            FD_ZERO(&readfds); 
            FD_SET(sockfd, &readfds); 
            timeout.tv_sec = TIMEOUT; 
            timeout.tv_usec = 0; 
            int activity = select(sockfd + 1, &readfds, NULL, NULL, &timeout); 
            if (activity > 0) { 
                if (FD_ISSET(sockfd, &readfds)) { 
                    recv(sockfd, &ack_pkt, sizeof(ack_pkt), 0); 
                    printf("Received ACK: %d\n", ack_pkt.seq_no); 
                    if (ack_pkt.seq_no >= base) { 
                        base = ack_pkt.seq_no + 1; 
                    } 
                } 
            } else { 
                printf("Timeout for packet: %d, retransmitting…\n", base); 
                pkt.seq_no = base; 
                send(sockfd, &pkt, sizeof(pkt), 0); 
                printf("Sent packet: %d\n", pkt.seq_no); 
            } 
        } 
    } 
    close(sockfd); 
    return 0; 
} 
