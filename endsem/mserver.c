#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

typedef struct files {
    char name[20];
    int count;
    char filename[100][20];
}fserver;

fserver f[4];
int main()
{

    // create a socket
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // define server address
    struct sockaddr_in server_add;
    server_add.sin_family = AF_INET;   // ip4
    server_add.sin_port = htons(9003); // port
    server_add.sin_addr.s_addr = INADDR_ANY;

    // bind
    bind(server_socket, (struct sockaddr *)&server_add, sizeof(server_add));

    // listen to 2 connections
    listen(server_socket, 2);
    int c=0;
    while(1){
    // accept
    int client_socket;

    client_socket = accept(server_socket, NULL, NULL);
    char sname[20];
    int numoffiles=0;
    recv(client_socket,sname, sizeof(sname), 0);
    recv(client_socket, &numoffiles, sizeof(int), 0);
    f[c]=(fserver*)(malloc(sizeof(fserver)));
    strcpy(f[c].name,sname);
    strcpy(f[c].count,numoffiles);
    for(int i=0;i<numoffiles;i++){
        char filename[100];
        int val = read(client_socket,filename, sizeof(filename));
        strcpy(f[c].filename[i],filename);
    }
    char message[20];
    strcpy(message,"Registered");
    write(client_socket, message, sizeof(message), 0);
    }
    close(server_socket);

    return 0;
}
