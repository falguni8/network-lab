#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main() {
    int client_socket;
    client_socket = socket(AF_INET,SOCK_DGRAM,0);
    if(client_socket < 0) {
        printf("Error while creating socket"); 
        exit(1);
    }

    struct sockaddr_in client_addr;
    socklen_t addr_size;
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(8002);
    client_addr.sin_addr.s_addr = INADDR_ANY;

    char buffer[1024];
    bzero(buffer,1024);
    printf("Enter The Domian Name\n");
    scanf("%[^\n]s",buffer); 
    printf("Client:%s\n",buffer);
    sendto(client_socket,buffer,sizeof(buffer),0,(struct sockaddr*)&client_addr,sizeof(client_addr));
    bzero(buffer,1024);
    addr_size = sizeof(client_addr);
    recvfrom(client_socket, buffer ,sizeof(buffer),0,(struct sockaddr*)&client_addr,&addr_size);
    printf("server:%s\n",buffer);
        
  
}
