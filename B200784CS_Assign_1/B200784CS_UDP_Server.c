#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main()
{
	// create a socket
	int server_socket;
	server_socket = socket(AF_INET, SOCK_DGRAM, 0);
	// define server address
	struct sockaddr_in server_add, client_add;
	server_add.sin_family = AF_INET;   // ip4
	server_add.sin_port = htons(9002); // port
	server_add.sin_addr.s_addr = INADDR_ANY;

	// bind
	bind(server_socket, (struct sockaddr *)&server_add, sizeof(server_add));

	memset(&client_add, 0, sizeof(client_add));
	char message[256];
	int len = sizeof(client_add);
	// read string sent by client
	recvfrom(server_socket, message, 256, MSG_WAITALL, (struct sockaddr *)&client_add, &len);
	int l = strlen(message);
	int i, j;
	for (i = 0, j = l - 1; i < j; i++, j--)
	{
		char c = message[i];
		message[i] = message[j];
		message[j] = c;
	}

	sendto(server_socket, message, 256, MSG_CONFIRM, (struct sockaddr *)&client_add, sizeof(client_add));
	close(server_socket);
	return 0;
}
