#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<signal.h>
#include<ctype.h>

volatile __sig_atomic_t flag = 0;
int network_socket;
char name[20];


void read_func()
{
    char msg[1000];

    while (1){
        memset(msg, 0, 1000);
        if (!read(network_socket, msg,strlen(msg)))
        {
            printf("Server disconnected\n");
            fflush(stdout);
            break;
        }
        printf("%s\n :", msg);
        fflush(stdout);
        memset(msg, 0, 1000);
    }
}

void write_func()
{
    char msg[1000];
    memset(msg, 0, 1000);
    while (1)
    {
        printf(":");
        fflush(stdout);
        fgets(msg,1000,stdin);
        msg[strlen(msg)-1]=0;
        if((strncmp(msg,"exit",4)==0)){
            write(network_socket, msg,strlen(msg));
            printf("Client Exits \n");
            break;
        }
        write(network_socket, msg,strlen(msg));
        memset(msg, 0, 1000);
    }
    flag=1;
}

int main()
{
    // create a socket
    network_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (network_socket < 0)
    {
        printf("Socket Creation Failed!");
        exit(0);
    }

    printf("Enter name :");
    fgets(name, 20, stdin);
    name[strlen(name) - 1] = 0;

    // specify an address for the socket
    struct sockaddr_in server_add;
    memset(&server_add,0,sizeof(server_add));
    server_add.sin_family = AF_INET;   // ip4
    server_add.sin_port = htons(9007); // port
    server_add.sin_addr.s_addr = INADDR_ANY;

    int connection_status = connect(network_socket, (struct sockaddr *)&server_add, sizeof(server_add));

    if (connection_status < 0)
    {
        printf("ERROR in connecting to socket\n");
        exit(0);
    }
    else
    {
        printf("Connection Established\n");
    }

    if(send(network_socket, name, sizeof(name),0)!=0){
        printf("Name not sent!\n");
        exit(1);
    }
    pthread_t read_t, write_t;
    pthread_create(&read_t, NULL, (void *)read_func, NULL);
    pthread_create(&write_t, NULL, (void *)write_func, NULL);
    while(1){
        if(flag){
            break;
        }
    }
    close(network_socket);
    return 0;
}
