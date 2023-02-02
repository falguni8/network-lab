#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

struct Client
{
    int socketfd, id;
    char name[20];
};
struct Client *c[10];
static int counter=0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void send_all(char *msg, int id)
{
    pthread_mutex_lock(&mutex);
    // for synchronization;

    for (int i = 0; i <counter; i++)
    {
        if (c[i] && c[i]->id != id)
        {
            write(c[i]->socketfd, msg, strlen(msg));
        }
    }
    pthread_mutex_unlock(&mutex);
}
void *func(void *client)
{
    // chat system r&w
    char buff[1000];
    int flag = 0;
    struct Client *p = (struct Client *)client;

    int n = read(p->socketfd,p->name,sizeof(p->name));
    printf("%s\n",p->name);
    if (!n)
    {
        flag = 1;
        printf("Read failed\n");
        exit(1);
    }
    memset(buff, 0, 1000);

    sprintf(buff,"%s came into the server", p->name);
    printf("%s\n", buff);

    send_all(buff, p->id);

    while (1)
    {
        memset(buff, 0, 1000);

        // read
        n = read(p->socketfd, buff, 1000);
        if (!n)
        {
            sprintf(buff, "%s Bye-Bye!\n", p->name);
            printf("%s\n", buff);
            break;
        }
        else
        {
            char msg[1000];
            memset(msg, 0, 1000);
            sprintf(msg,"%s : %s",p->name,buff);
            send_all(msg, p->id);
            printf("%s\n", msg);
        }
    }
    close(p->socketfd);
    c[p->id] = NULL;
    free(p);
    pthread_detach(pthread_self());
    return 0;
}

int main()
{

    // create a socket
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket < 0)
    {
        printf("Socket Creation Failed!\n");
        exit(1);
    }
    else{
        printf("Socket Created ...\n");
    }
    // define server address
    struct sockaddr_in server_add;
    server_add.sin_family = AF_INET;   // ip4
    server_add.sin_port = htons(9003); // port
    server_add.sin_addr.s_addr = INADDR_ANY;

    // for reuse address
    int option = 1;
    if (setsockopt(server_socket, SOL_SOCKET, (SO_REUSEPORT | SO_REUSEADDR), (char *)&option, sizeof(option)) < 0)
    {
        printf("setsockopt failed!\n");
        exit(1);
    }

    // bind
    int bindstatus = bind(server_socket, (struct sockaddr *)&server_add, sizeof(server_add));
    if (bindstatus!=0)
    {
        printf("Binding Failed!\n");
        exit(1);
    }
    else{
        printf("Socket binded ..\n");
    }

    // listen to 2 connections
    int listenstatus = listen(server_socket, 10);

    if (listenstatus < 0)
    {
        printf("Listen Failed!\n");
        exit(1);
    }
    else{
        printf("Server is listening\n");
    }
    pthread_t tid;
    while (1)
    {
        // accept
        int client_socket;
        client_socket = accept(server_socket,(struct sockaddr *)&server_add,sizeof(server_add));
        if (client_socket < 0 || counter == 9)
        {
            printf("%d\n",counter);
            printf("Error in accepting  or Max Limit!\n");
            close(client_socket);
            continue;
        }
        else
        {
            printf("New Client Joined\n");
            c[counter] = (struct Client *)malloc(sizeof(struct Client));
            c[counter]->socketfd = client_socket;
            c[counter]->id = counter;
            pthread_create(&tid, NULL, (void *)&func, (void *)c[10]);
            counter++;
        }
    }
    close(server_socket);
    return 0;
}
