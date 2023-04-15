#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

typedef struct
{
    unsigned char type;
    unsigned short seq_num;
    unsigned char payload[1024];
    unsigned char trailer[1];
} packet_t;

unsigned char calculate_checksum(unsigned char *packet, size_t packet_size)
{
    unsigned char checksum = 0;
    printf("%d\n",sizeof(packet));
    for (size_t i = 0; i < packet_size; i++)
    {
        checksum ^= packet[i];
    }
    printf("%u\n",checksum);
    return checksum;
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
    pthread_mutex_t mutex;
    if (pthread_mutex_init(&mutex, NULL) != 0)
    {
        perror("pthread_mutex_init");
        exit(1);
    }
    struct sockaddr_in receiver_addr;
    memset(&receiver_addr, 0, sizeof(receiver_addr));
    receiver_addr.sin_family = AF_INET;
    receiver_addr.sin_port = htons(1234);
    receiver_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int seq_number = 0;
    while (1)
    {
        // Send packet type 1 every 100ms
        pthread_mutex_lock(&mutex);
        packet_t packet_type_1;
        packet_type_1.type = 1;
        packet_type_1.seq_num = seq_number;
        memset(packet_type_1.payload,"TYPE1", sizeof("TYPE1"));
        packet_type_1.trailer[0] = calculate_checksum((unsigned char *)&packet_type_1, sizeof(packet_t));
        ssize_t bytes_sent = sendto(sockfd, &packet_type_1, sizeof(packet_t), 0, (struct sockaddr *)&receiver_addr, sizeof(receiver_addr));
        if (bytes_sent < 0)
        {
            perror("sendto");
        }
        seq_number++;
        
        usleep(100000);
        pthread_mutex_unlock(&mutex);
        pthread_mutex_lock(&mutex);
        // Send packet type 2 every 150ms
        packet_t packet_type_2;
        packet_type_2.type = 2;
        packet_type_2.seq_num = seq_number;
        memset(packet_type_2.payload,"TYPE2", sizeof("TYPE2"));
        packet_type_2.trailer[0] = calculate_checksum((unsigned char *)&packet_type_2, sizeof(packet_t));
        bytes_sent = sendto(sockfd, &packet_type_2, sizeof(packet_t), 0, (struct sockaddr *)&receiver_addr, sizeof(receiver_addr));
        if (bytes_sent < 0)
        {
            perror("sendto");
        }
        seq_number++;
        usleep(150000);
        pthread_mutex_unlock(&mutex);
    }

    // Close the socket
    close(sockfd);

    return 0;
}

