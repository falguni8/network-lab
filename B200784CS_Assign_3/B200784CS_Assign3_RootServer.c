#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>



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
  server_addr.sin_port = htons(9003);
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  // Bind socket to address
  if (bind(sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
    perror("Error binding socket");
    exit(1);
  }

  while (1) {

    len = sizeof(client_addr);
    if (recvfrom(sock, buffer, 1024, 0, (struct sockaddr *) &client_addr, &len) == -1) {
      perror("Error receiving data");
      exit(1);
    }

    printf("Received request for domain: %s\n", buffer);

    char TLD[8];
int i=0,j=0;
int count =0;
for( i=0; buffer[i]!='\0'; i++){
	
	if(count >=2) {
	   TLD[j]=buffer[i];
	   j++;
	}
	if(buffer[i]=='.') count++;
}
TLD[j]='\0';

if(strcmp("com",TLD)==0){
  printf("Directed to TLD(COM) server\n");
	int TLD_COM;
    TLD_COM = socket(AF_INET,SOCK_DGRAM,0);
    if(TLD_COM < 0) {
        printf("Error while creating socket"); 
        exit(1);
    }

    struct sockaddr_in TLD_COM_ADDR;
    socklen_t TLD_COM_ADDR_SIZE;
    TLD_COM_ADDR.sin_family = AF_INET;
    TLD_COM_ADDR.sin_port = htons(9004);////have to change
    TLD_COM_ADDR.sin_addr.s_addr = INADDR_ANY;
    
    
    sendto(TLD_COM,buffer,sizeof(buffer),0,(struct sockaddr*)&TLD_COM_ADDR,sizeof(TLD_COM_ADDR));
    bzero(buffer,1024);
    TLD_COM_ADDR_SIZE = sizeof(TLD_COM_ADDR);
    recvfrom(TLD_COM, buffer ,sizeof(buffer),0,(struct sockaddr*)&TLD_COM_ADDR,&TLD_COM_ADDR_SIZE);
    printf("server:%s\n",buffer);
    
    
    sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *) &client_addr, len);

}else if(strcmp("in",TLD)==0){
  printf("Directed to TLD(IN) server\n");
	int TLD_IN;
    TLD_IN = socket(AF_INET,SOCK_DGRAM,0);
    if(TLD_IN < 0) {
        printf("Error while creating socket"); 
        exit(1);
    }

    struct sockaddr_in TLD_IN_ADDR;
    socklen_t TLD_IN_ADDR_SIZE;
    TLD_IN_ADDR.sin_family = AF_INET;
    TLD_IN_ADDR.sin_port = htons(12005);////have to change
    TLD_IN_ADDR.sin_addr.s_addr = INADDR_ANY;
    
    
    sendto(TLD_IN,buffer,sizeof(buffer),0,(struct sockaddr*)&TLD_IN_ADDR,sizeof(TLD_IN_ADDR));
    bzero(buffer,1024);
    TLD_IN_ADDR_SIZE = sizeof(TLD_IN_ADDR);
    recvfrom(TLD_IN, buffer ,sizeof(buffer),0,(struct sockaddr*)&TLD_IN_ADDR,&TLD_IN_ADDR_SIZE);
    printf("server:%s\n",buffer);
    
    
    sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *) &client_addr, len);

}else if(strcmp("org",TLD)==0){
	printf("Directed to TLD(ORG) server\n");
	int TLD_ORG;
    TLD_ORG = socket(AF_INET,SOCK_DGRAM,0);
    if(TLD_ORG < 0) {
        printf("Error while creating socket"); 
        exit(1);
    }

    struct sockaddr_in TLD_ORG_ADDR;
    socklen_t TLD_ORG_ADDR_SIZE;
    TLD_ORG_ADDR.sin_family = AF_INET;
    TLD_ORG_ADDR.sin_port = htons(9006);////have to change
    TLD_ORG_ADDR.sin_addr.s_addr = INADDR_ANY;
    
    
    sendto(TLD_ORG,buffer,sizeof(buffer),0,(struct sockaddr*)&TLD_ORG_ADDR,sizeof(TLD_ORG_ADDR));
    bzero(buffer,1024);
    TLD_ORG_ADDR_SIZE = sizeof(TLD_ORG_ADDR);
    recvfrom(TLD_ORG, buffer ,sizeof(buffer),0,(struct sockaddr*)&TLD_ORG_ADDR,&TLD_ORG_ADDR_SIZE);
    printf("TLD(ORG) server:%s\n",buffer);
    
    
    sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *) &client_addr, len);
    
}else{
	printf("This program have limited capability hence this TLD is not yet implemented\n");
}
}

  close(sock);
  return 0;
}

