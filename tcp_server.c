#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

int main() {

    

    //create a socket
    int server_socket;
    server_socket=socket(AF_INET,SOCK_STREAM,0);

    //define server address
    struct sockaddr_in server_add;
    server_add.sin_family=AF_INET;//ip4
    server_add.sin_port=htons(9002);//port
    server_add.sin_addr.s_addr=INADDR_ANY;

    //bind
    bind(server_socket,(struct sockaddr *)&server_add,sizeof(server_add));

    //listen
    listen(server_socket,2);
    
    //accept
    int client_socket;

    client_socket=accept(server_socket,NULL,NULL);
    
    
    char message[256];
    //read string sent by client
    int val=read(client_socket,message,sizeof(message));
    printf("%s\n",message);
    int l=strlen(message);
    int i,j;
    for(i=0, j=l-1;i<j;i++,j--){
    	char c=message[i];
    	message[i]=message[j];
    	message[j]=c;
    }
    printf("%s\n",message);
    send(client_socket,message,sizeof(message),0);
    close(server_socket);
    return 0;

}
