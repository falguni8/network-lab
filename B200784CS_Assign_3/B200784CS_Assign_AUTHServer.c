#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
struct ns_record {
    char name[100];
    char type[10];
    char value[100];
};

int compare(char buffer[],struct ns_record ns[] ){
    for(int i=0; i<8; i++){
        if(strcmp(ns[i].name,buffer)==0) return i;
    }
    return -1;
}

int main() {
    int server_socket,client_socket;
    server_socket = socket(AF_INET,SOCK_DGRAM,0);
    if(server_socket < 0) {
        printf("Error while creating socket"); 
        exit(1);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(1050);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    int n = bind(server_socket,(struct sockaddr*)&server_addr,sizeof(server_addr));
    if(n < 0) {
        printf("Error while binding"); 
        exit(1);
    }
    

     struct ns_record ns[10];

    strcpy(ns[0].name, "ns1.example.in");
    strcpy(ns[0].type, "A");
    strcpy(ns[0].value, "32.213.2.0");

    strcpy(ns[1].name, "ns2.example.in");
    strcpy(ns[1].type, "A");
    strcpy(ns[1].value, "122.43.12.1");

    strcpy(ns[2].name, "ns1.abc.in");
    strcpy(ns[2].type, "A");
    strcpy(ns[2].value, "121.53.99.7");

    strcpy(ns[3].name, "ns1.pqr.in");
    strcpy(ns[3].type, "A");
    strcpy(ns[3].value, "84.234.21.3");

    strcpy(ns[4].name, "ns2.pqr.in");
    strcpy(ns[4].type, "A");
    strcpy(ns[4].value, "98.32.43.1");

    strcpy(ns[5].name, "ns1.google.in");
    strcpy(ns[5].type, "A");
    strcpy(ns[5].value, "82.123.134.4");

    strcpy(ns[6].name, "ns2.google.in");
    strcpy(ns[6].type, "A");
    strcpy(ns[6].value, "64.231.21.3");

    strcpy(ns[7].name, "www.facebook.in");
    strcpy(ns[7].type, "A");
    strcpy(ns[7].value, "157.240.22.35");

    while(1){
    	socklen_t addr_size;
    	struct sockaddr_in client_addr;
    	addr_size = sizeof(client_addr);
        char buffer[1024];
        bzero(buffer,1024);
        recvfrom(server_socket,buffer,1024,0,(struct sockaddr*)&client_addr,&addr_size);
        printf("Request from TLD server for Domain name:%s\n",buffer);
        int idx = compare(buffer,ns);
        if(idx!=-1){
            bzero(buffer,1024);
            strcpy(buffer,ns[idx].value);
            printf("Authoritative server response: %s",buffer);
            sendto(server_socket,buffer,sizeof(buffer),0,(struct sockaddr*)&client_addr,sizeof(client_addr));
            printf("\n");
        }
        else{
            bzero(buffer,1024);
            strcpy(buffer,"NOT FOUND");
            printf("Authoritative server response: %s",buffer);
            sendto(server_socket,buffer,sizeof(buffer),0,(struct sockaddr*)&client_addr,sizeof(client_addr));
            printf("\n");
        }
        
    }
    return 0;
}
