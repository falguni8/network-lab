#include<stdio.h>
#include<stdlib.h>

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

int main() {
	//create a socket
	int network_socket;
    	network_socket=socket(AF_INET,SOCK_DGRAM,0);

	//specify an address for the socket
	struct sockaddr_in server_add;
	server_add.sin_family=AF_INET;//ip4
	server_add.sin_port=htons(9002);//port
	server_add.sin_addr.s_addr=INADDR_ANY;
	
	
	//receive data from server
	char message[256];
	scanf("%s",message);
	int l=strlen(message);
	//send to server
	sendto(network_socket,message,256,0,(struct sockaddr*)&server_add,sizeof(server_add));
	
	int val=recvfrom(network_socket,message,sizeof(message),MSG_WAITALL,(struct sockaddr*)&server_add,sizeof(server_add));
	printf("%s\n",message);
	close(network_socket);
	return 0;
}
