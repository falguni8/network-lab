#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PACKET_SIZE 1026 // packet size = payload(1024) + type(1) + seq_num(1) + checksum(1)
#define TYPE1_PACKET 1
#define TYPE2_PACKET 2

typedef struct packet {
    int type;
    int seq_num;
    char payload[1024];
    char checksum;
} packet_t;

// function prototypes for thread functions
void *error_check_thread(void *arg);
void *type1_packet_thread(void *arg);
void *type2_packet_thread(void *arg);
void *print_packet_count_thread(void *arg);

int main() {
    int sock, port;
    struct sockaddr_in server_addr, client_addr;
    pthread_t threads[4];

    // Create a UDP socket
    if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        perror("Error creating socket");
        exit(1);
    }

    // Set socket options
    int broadcast_enable = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast_enable, sizeof(broadcast_enable)) == -1) {
        perror("Error setting socket options");
        exit(1);
    }

    // Bind socket to port
    port = 12345; // replace with desired port number
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    if (bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error binding socket to port");
        exit(1);
    }

    // Create threads
    if (pthread_create(&threads[0], NULL, error_check_thread, (void *)&sock) != 0) {
        perror("Error creating error check thread");
        exit(1);
    }

    if (pthread_create(&threads[1], NULL, type1_packet_thread, (void *)&sock) != 0) {
        perror("Error creating type1 packet thread");
        exit(1);
    }

    if (pthread_create(&threads[2], NULL, type2_packet_thread, (void *)&sock) != 0) {
        perror("Error creating type2 packet thread");
        exit(1);
    }

    if (pthread_create(&threads[3], NULL, print_packet_count_thread, (void *)&sock) != 0) {
        perror("Error creating print packet count thread");
        exit(1);
    }

    // Wait for threads to finish
    for (int i = 0; i < 4; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Error joining thread");
            exit(1);
        }
    }

    return 0;
}

// thread function implementations

void *error_check_thread(void *arg) {
    int sock = *(int *)arg;
    packet_t packet;
    int recv_len;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    while (1) {
        memset(&packet, 0, sizeof(packet));

        // Receive packet from client
        if ((recv_len = recvfrom(sock, &packet, PACKET_SIZE, 0, (struct sockaddr *)&client_addr, &client_len)) == -1) {
            perror("Error receiving packet");
            continue;
        }

        // Check checksum
        char checksum = packet.payload[0];
        for (int i = 1; i < 1024; i++) {
            checksum ^= packet.payload[i];
        }
        if (checksum != packet.checksum) {
            printf("Error: Checksum mismatch for packet %d\n", packet.seq_num);
            continue;
        }

        // Packet is error-free, proceed to type-specific processing threads
        if (packet.type == TYPE1_PACKET) {
            // send to type1 packet thread
        } else if (packet.type == TYPE2_PACKET) {
            // send to type2 packet thread
        } else {
            printf("Error: Unknown packet type %d for packet %d\n", packet.type, packet.seq_num);
            continue;
        }
    }
    return NULL;
}


void *type1_packet_thread(void *arg) {
    int sock = *(int *)arg;
    packet_t packet;
    int recv_len;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int num_type1_packets = 0;

    while (1) {
        memset(&packet, 0, sizeof(packet));

        // Receive packet from client
        if ((recv_len = recvfrom(sock, &packet, PACKET_SIZE, 0, (struct sockaddr *)&client_addr, &client_len)) == -1) {
            perror("Error receiving packet");
            continue;
        }

        // Check packet type
        if (packet.type != TYPE1_PACKET) {
            printf("Error: Received packet with incorrect type %d in type1_packet_thread\n", packet.type);
            continue;
        }

        // Check checksum
        char checksum = packet.payload[0];
        for (int i = 1; i < 1024; i++) {
            checksum ^= packet.payload[i];
        }
        if (checksum != packet.checksum) {
            printf("Error: Checksum mismatch for type 1 packet %d\n", packet.seq_num);
            continue;
        }

        // Packet is error-free, increment counter
        num_type1_packets++;
    }

    return NULL;
}

void *type2_packet_thread(void *arg) {
    int sock = *(int *)arg;
    packet_t packet;
    int recv_len;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int num_type2_packets = 0;

    while (1) {
        memset(&packet, 0, sizeof(packet));

        // Receive packet from client
        if ((recv_len = recvfrom(sock, &packet, PACKET_SIZE, 0, (struct sockaddr *)&client_addr, &client_len)) == -1) {
            perror("Error receiving packet");
            continue;
        }

        // Check packet type
        if (packet.type != TYPE2_PACKET) {
            printf("Error: Received packet with incorrect type %d in type2_packet_thread\n", packet.type);
            continue;
        }

        // Check checksum
        char checksum = packet.payload[0];
        for (int i = 1; i < 1024; i++) {
            checksum ^= packet.payload[i];
        }
        if (checksum != packet.checksum) {
            printf("Error: Checksum mismatch for type 2 packet %d\n", packet.seq_num);
            continue;
        }

        // Packet is error-free, increment counter
        num_type2_packets++;
    }

    return NULL;
}

void *print_packet_count_thread(void *arg) {
    int num_type1_packets = 0;
    int num_type2_packets = 0;

    while (1) {
        // Sleep for 300 ms
        usleep(300000);

        // Print packet counts
        printf("Number of type 1 packets received: %d\n", num_type1_packets);
        printf("Number of type 2 packets received: %d\n", num_type2_packets);
    }

    return NULL;
}

