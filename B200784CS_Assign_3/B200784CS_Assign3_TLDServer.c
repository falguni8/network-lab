#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

struct ns_record {
    char name[100];
    char type[10];
    char value[100];
};


int compare(char buffer[],struct ns_record ns[] ){
  printf("Inside the compare fun \n");
    for(int i=0; i<8; i++){
        printf("%d ",i);
        if(strcmp(ns[i].name,buffer)==0) return i;
    }
    return -1;
}

int main(int argc, char *argv[]) {
  int sock, client_sock, len;
  struct sockaddr_in server_addr, client_addr;
  char buffer[1024];

  // Create socket
  if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    perror("Error creating socket");
    exit(1);
  }

  // Set server address
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(12005);
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  // Bind socket to address
  if (bind(sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
    perror("Error binding socket");
    exit(1);
  }

 struct ns_record ns[10];

 strcpy(ns[0].name, "www.example.in");
 strcpy(ns[0].type, "NS");
 strcpy(ns[0].value, "ns1.example.in");

 strcpy(ns[1].name, "www.example.in");
 strcpy(ns[1].type, "NS");
 strcpy(ns[1].value, "ns2.example.in");

 strcpy(ns[2].name, "www.abc.in");
 strcpy(ns[2].type, "NS");
 strcpy(ns[2].value, "ns1.abc.in");

 strcpy(ns[3].name, "www.pqr.in");
 strcpy(ns[3].type, "NS");
 strcpy(ns[3].value, "ns1.pqr.in");

 strcpy(ns[4].name, "www.pqr.com");
 strcpy(ns[4].type, "NS");
 strcpy(ns[4].value, "ns2.pqr.in");

 strcpy(ns[5].name, "www.google.in");
 strcpy(ns[5].type, "NS");
 strcpy(ns[5].value, "ns1.google.in");

 strcpy(ns[6].name, "www.google.in");
 strcpy(ns[6].type, "NS");
 strcpy(ns[6].value, "ns2.google.in");

 strcpy(ns[7].name, "www.facebook.in");
 strcpy(ns[7].type, "A");
 strcpy(ns[7].value, "157.240.22.35");
  while (1) {
    len = sizeof(client_addr);
    if (recvfrom(sock, buffer, 1024, 0, (struct sockaddr *) &client_addr, &len) == -1) {
      perror("Error receiving data");
      exit(1);
    }

    printf("Received request for domain: %s\nChecking in the NS and A reords\n", buffer);
    printf("Before compare function\n");
    int idx = compare(buffer,ns);
    
    if(idx!=-1){
        
        if(strcmp(ns[idx].type,"A")==0) {
            bzero(buffer,1024);
            strcpy(buffer,ns[idx].value);
            
            if (sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *) &client_addr, len) == -1) {
            perror("Error sending data");
            exit(1);
            }
        }
        else{
            //printf("Inside else\n");
            
            printf("Sending request to authoritative server \n");
          

            int sock1, client_sock1, len1;
            struct sockaddr_in server_addr1, client_addr1;
            char buffer1[1024];

            // Create socket
            if ((sock1 = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
            perror("Error creating socket");
            exit(1);
            }

            // Set server address
            server_addr1.sin_family = AF_INET;
            server_addr1.sin_port = htons(1050);
            server_addr1.sin_addr.s_addr = htonl(INADDR_ANY);
            bzero(buffer,1024);
            strcpy(buffer,ns[idx].value);
            sendto(sock1,buffer,sizeof(buffer),0,(struct sockaddr*)&server_addr1,sizeof(server_addr1));
            
            bzero(buffer,1024);
            len1 = sizeof(server_addr1);
            recvfrom(sock1, buffer ,sizeof(buffer),0,(struct sockaddr*)&server_addr1,&len1);

            if(strcmp(buffer,"Not found")==0)
            printf("Sending error message: %s\n", buffer);
            else{
            printf("Sending local IP: %s\n", buffer);
            
            }
            if (sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *) &client_addr, len) == -1) {
            perror("Error sending data");
            exit(1);
            }
        }
    }
    else{
        bzero(buffer,1024);
        strcpy(buffer,"NOT FOUND");
        printf("Required domain name in not present in the TLD NS record\n");
        if (sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *) &client_addr, len) == -1) {
        perror("Error sending data");
        exit(1);
        }
    }
    
  }

  close(sock);
  return 0;
}

