#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void register()
{
    int network_socket;
    network_socket = socket(AF_INET, SOCK_STREAM, 0);

    // specify an address for the socket
    struct sockaddr_in server_add;
    server_add.sin_family = AF_INET;   // ip4
    server_add.sin_port = htons(9003); // port
    server_add.sin_addr.s_addr = INADDR_ANY;

    int connection_status = connect(network_socket, (struct sockaddr *)&server_add, sizeof(server_add));

    if (connection_status < 0)
    {
        printf("ERROR in connecting to socket\n");
    }
    int d = 0;
    send(network_socket, "CSE", strlen("CSE"), 0);
    d = 3;
    send(network_socket, &d, sizeof(d), 0);
    send(network_socket, "CSE/file1.txt", strlen("CSE/file1.txt"), 0);
    send(network_socket, "CSE/file2.txt", strlen("CSE/file2.txt"), 0);
    send(network_socket, "CSE/file3.txt", strlen("CSE/file3.txt"), 0);
}

int main()
{

    // create a socket
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // define server address
    struct sockaddr_in server_add;
    server_add.sin_family = AF_INET;   // ip4
    server_add.sin_port = htons(9002); // port
    server_add.sin_addr.s_addr = INADDR_ANY;

    // bind
    bind(server_socket, (struct sockaddr *)&server_add, sizeof(server_add));

    // listen to 2 connections
    listen(server_socket, 2);
    registerserver();
    while (1)
    {
        // accept
        int client = accept(server_socket, NULL, NULL);
        char buffer[256];
        recv(client_socket, buffer, 256, 0);

        // Handle file requests
        if (strcmp(buffer, "FILE_LIST") == 0)
        {
            // Send list of files
            send(client_socket, "file1.txt\nfile2.txt\nfile3.txt\n", strlen("file1.txt\nfile2.txt\nfile3.txt\n"), 0);
        }
        else
        {
            // Send file
            char file_path[20];
            sprintf(file_path, "%s/%s", department_name, buffer);

            FILE *file = fopen(file_path, "r");
            char file_content[256];
            fgets(file_content, 256, file);

            send(client_socket, file_content, strlen(file_content), 0);

            fclose(file);
        }
    }
    close(server_socket);
    return 0;
}
