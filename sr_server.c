
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <arpa/inet.h> 
#include <time.h> 
#define PORT 8080 
#define MAX_CLIENTS 4 
#define LOSS_PROBABILITY 10 
 
typedef struct { 
    int seq_no; 
    char data[1024]; 
} packet; 
int simulate_packet_loss() { 
    return rand() % 100 < LOSS_PROBABILITY; 
} 
int main() { 
    int server_fd, new_sock; 
    struct sockaddr_in server_addr, client_addr; 
    socklen_t addr_size; 
    packet pkt, ack_pkt;  
    srand(time(NULL));  
    server_fd = socket(AF_INET, SOCK_STREAM, 0); 
    if (server_fd < 0) { 
        perror("Socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
    server_addr.sin_family = AF_INET; 
    server_addr.sin_port = htons(PORT); 
    server_addr.sin_addr.s_addr = INADDR_ANY; 
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) { 
        perror("Bind failed"); 
        close(server_fd); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, MAX_CLIENTS) < 0) { 
        perror("Listen failed"); 
        close(server_fd); 
        exit(EXIT_FAILURE); 
    } 
    printf("Server listening on port %d…\n", PORT); 
    addr_size = sizeof(client_addr); 
    new_sock = accept(server_fd, (struct sockaddr*)&client_addr, &addr_size); 
    if (new_sock < 0) { 
        perror("Accept failed"); 
        close(server_fd); 
        exit(EXIT_FAILURE); 
    } 
    while (1) { 
        if (recv(new_sock, &pkt, sizeof(pkt), 0) <= 0) { 
            break; 
        } 
        printf("Received packet: %d\n", pkt.seq_no); 
         
        if (simulate_packet_loss()) { 
            printf("Packet %d lost, no ACK sent\n", pkt.seq_no); 
            continue; 
        }     
        ack_pkt.seq_no = pkt.seq_no; 
        send(new_sock, &ack_pkt, sizeof(ack_pkt), 0); 
        printf("Sent ACK for seq_no: %d\n", ack_pkt.seq_no); 
    } 
    close(new_sock); 
    close(server_fd); 
    return 0; 
} 
