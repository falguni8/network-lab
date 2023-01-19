#include<stdio.h>
#include<stdlib.h>

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

int main() {
    //create a socket
    int network_socket;
    network_socket=socket(AF_INET,SOCK_STREAM,0);

    //specify an address for the socket
    struct sockaddr_in server_add;
    server_add.sin_family=AF_INET;//ip4
    server_add.sin_port=htons(9002);//port
    server_add.sin_addr.s_addr=INADDR_ANY;

    int connection_status=connect(network_socket,(struct sockaddr*)&server_add,sizeof(server_add));

    if(connection_status<0){
        printf("ERROR in connecting to socket\n");
    }

    //receive data from server

    char message[256];
    scanf("%s",message);
    int l=strlen(message);
    //send tto server
    write(network_socket,message,sizeof(message),0);
    int val=read(network_socket,message,sizeof(message));
    printf("%s\n",message);
    close(network_socket);
    return 0;

}
