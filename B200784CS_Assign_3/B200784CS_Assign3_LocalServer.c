#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>





int max_capacity = 10;

char HOST_NAME[10][1024];
int topOfHostName = 0;

char ip_addr[10][1024];
int topOfIpAddr = 0;



int compare(char buffer[]){
if(topOfHostName == 0) return -1;
for(int i=0; i<topOfHostName; i++){
if(strcmp(buffer,HOST_NAME[i])==0) return i;
}
return -1;
}

void addInfo(char buffer1[],char buffer[]){
if(topOfHostName >= max_capacity) return ;
strcpy(HOST_NAME[topOfHostName],buffer1);
strcpy(ip_addr[topOfIpAddr],buffer);
topOfHostName++;
topOfIpAddr++;
return ;
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
server_addr.sin_port = htons(8002);
server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

// Bind socket to address
if (bind(sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
perror("Error binding server socket");
exit(1);
}

while (1) {
len = sizeof(client_addr);
if (recvfrom(sock, buffer, 1024, 0, (struct sockaddr *) &client_addr, &len) == -1) {
perror("Error receiving data");
exit(1);
}

printf("Received request for domain: %s\n", buffer);
if (compare(buffer)!=-1) {
int idx = compare(buffer);
strcpy(buffer,ip_addr[idx]);

printf("Sending local IP: %s\n", buffer);
if (sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *) &client_addr, len) == -1) {
perror("Error sending data");
exit(1);
}
} else {

printf("Checking into root server\n");
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
server_addr1.sin_port = htons(9003);
server_addr1.sin_addr.s_addr = htonl(INADDR_ANY);


sendto(sock1,buffer,sizeof(buffer),0,(struct sockaddr*)&server_addr1,sizeof(server_addr1));
strcpy(buffer1,buffer);
bzero(buffer,1024);
len1 = sizeof(server_addr1);
recvfrom(sock1, buffer ,sizeof(buffer),0,(struct sockaddr*)&server_addr1,&len1);

if(strcmp(buffer,"Not found")==0)
printf("Sending error message: %s\n", buffer);
else{
printf("Sending local IP: %s\n", buffer);
addInfo(buffer1,buffer);
}
if (sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *) &client_addr, len) == -1) {
perror("Error sending data");
exit(1);
}



}
}

close(sock);
return 0;
}

