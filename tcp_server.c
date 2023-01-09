#include<stdio.h>
#include<stdlib.h>

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

int main() {

    char server_message[256]="You have reached server";

    //create a socket
    int server_socket;
    server_socket=socket(AF_INET,SOCK_STREAM,0);

    //define server address
    struct sockaddr_in server_add;
    server_add.sin_family=AF_INET;//ip4
    server_add.sin_port=htons(9002);//port
    server_add.sin_addr.s_addr=INADDR_ANY;

    //bind
    bind(server_socket,(struct *sockaddr)&server_add,sizeof(server_add));

    //listen
    listen(server_socket,2);
    
    //accept
    int client_socket;

    client_socket=accept(server_socket,NULL,NULL);
    
    //send

    send(client_socket,server_message,sizeof(server_message));
     
    //receive data from server

    char server_response[256];
    recv(network_socket,&server_response,sizeof(server_response),0);

    //output

    printf("%s\n",server_response);

    close(server_socket);
    return 0;

}