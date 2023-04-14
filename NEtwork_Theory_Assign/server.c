#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include<stdbool.h>

#define MAX_PACKET_SIZE 1024

typedef struct
{
    unsigned char type;
    unsigned short seq_num;
    unsigned char payload[1024];
    unsigned char trailer[1];
} packet_t;

typedef struct
{
    int type1_packets_received;
    int type2_packets_received;
} packet_count_t;

// Function prototypes

int count_type1 = 0, count_type2 = 0;
pthread_mutex_t mutex;

unsigned char calculate_checksum(unsigned char *packet, size_t packet_size)
{
    unsigned char checksum = 0;

    for (size_t i = 0; i < packet_size; i++)
    {
        checksum ^= packet[i];
    }

    return checksum;
}

void *error_check_thread(void *arg)
{
    int sockfd = *(int *)arg;
    packet_t received_packet;
    ssize_t bytes_received;
    bool error;

    while (true)
    {
        // Receive a packet
        bytes_received = recv(sockfd, &received_packet, sizeof(packet_t), 0);
        if (bytes_received < 0)
        {
            perror("recv");
            continue;
        }

        // Check for errors
        error = (calculate_checksum((unsigned char *)&received_packet, sizeof(packet_t)) != received_packet.trailer[0]);
        if (error)
        {
            printf("Packet type %d, sequence number %d, contains errors\n", received_packet.type, received_packet.seq_num);
        }
    }
}

void *type1_process_thread(void *arg)
{
    int sockfd = *(int *)arg;
    packet_t received_packet;
    ssize_t bytes_received;

    while (true)
    {
        // Receive a packet
        bytes_received = recv(sockfd, &received_packet, sizeof(packet_t), 0);
        if (bytes_received < 0)
        {
            perror("recv");
            continue;
        }

        // Check if packet is of type 1
        if (received_packet.type != 1)
        {
            continue;
        }

        // Process packet of type 1
        printf("Processing packet type 1, sequence number %d\n", received_packet.seq_num);
        count_type1++;
    }
}

void *type2_process_thread(void *arg)
{
    int sockfd = *(int *)arg;
    packet_t received_packet;
    ssize_t bytes_received;

    while (true)
    {
        // Receive a packet
        bytes_received = recv(sockfd, &received_packet, sizeof(packet_t), 0);
        if (bytes_received < 0)
        {
            perror("recv");
            continue;
        }

        // Check if packet is of type 2
        if (received_packet.type != 2)
        {
            continue;
        }

        // Process packet of type 2
        printf("Processing packet type 2, sequence number %d\n", received_packet.seq_num);
        count_type2++;
    }
}



void *packet_count_thread(void *arg)
{
    int sockfd = *(int *)arg;
    
    
    while (1)
    {
        printf("Number of type 1 packets received: %d\n", count_type1);
        printf("Number of type 2 packets received: %d\n", count_type2);
        fflush(stdout);
        sleep(3);
    }
    
    return NULL;
}

int main()
{
    // Create a UDP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("socket");
        exit(1);
    }

    // Set up the address structure for the receiver
    struct sockaddr_in receiver_addr;
    memset(&receiver_addr, 0, sizeof(receiver_addr));
    receiver_addr.sin_family = AF_INET;
    receiver_addr.sin_port = htons(1234);
    receiver_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the receiver address
    if (bind(sockfd, (struct sockaddr *)&receiver_addr, sizeof(receiver_addr)) < 0)
    {
        perror("bind");
        exit(1);
    }

    // Create a mutex to synchronize access to the packet count variables
    
    if (pthread_mutex_init(&mutex, NULL) != 0)
    {
        perror("pthread_mutex_init");
        exit(1);
    }

    // Initialize the packet count variables
    int type1_packet_count = 0;
    int type2_packet_count = 0;

    // Create the threads
    pthread_t threads[4];

    if (pthread_create(&threads[0], NULL, error_check_thread, &sockfd) != 0)
    {
        perror("pthread_create");
        exit(1);
    }

    if (pthread_create(&threads[1], NULL, type1_process_thread, &sockfd) != 0)
    {
        perror("pthread_create");
        exit(1);
    }

    if (pthread_create(&threads[2], NULL, type2_process_thread, &sockfd) != 0)
    {
        perror("pthread_create");
        exit(1);
    }

    if (pthread_create(&threads[3], NULL, packet_count_thread, &mutex) != 0)
    {
        perror("pthread_create");
        exit(1);
    }

    // Wait for the threads to finish
    for (int i = 0; i < 4; i++)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            perror("pthread_join");
            exit(1);
        }
    }

    // Destroy the mutex
    pthread_mutex_destroy(&mutex);

    // Close the socket
    close(sockfd);

    return 0;
}

