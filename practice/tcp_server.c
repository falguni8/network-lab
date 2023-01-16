#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

typedef struct fruit{
    char name[20];
    int count;
}

int main() {

    struct fruit F[5];
    strcpy(F[0].name,"apple");
    F[0].count=10;

    strcpy(F[0].name,"mango");
    F[1].count=10;

    strcpy(F[0].name,"chikoo");
    F[2].count=10;

    strcpy(F[0].name,"papaya");
    F[3].count=10;

    strcpy(F[0].name,"banana");
    F[4].count=10;

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
    
    
    char message[20];
    //read string sent by client
    int val=read(client_socket,message,sizeof(message));

    while(1){
        if(strcmp(message,"Fruits")==0){
            message="Enter the name of fruit and count";
            fruit fuser;
            scanf("%s%d",fuser.name,fuser.count);
            for(int i=0;i<5;i++){
                if(strcmp(fuser.name,F[i].name)==0){
                    if(fuser.count>fuser.count){
                        printf("NOT AVAILABLE\n");
                    }
                    else{
                        struct F[i].count-=fuser.count;
                    }
                }
            }
        }
        else if(strcmp(message,"SendInventory")==0){
            for(int i=0;i<5;i++){
                printf("%s %d\n",F[i].name,F[i].count);
            }
        }
        else if(strcmp(message,"Exit")==0){
            printf("SERVER EXIT\n")
            break;
        }
        else{
            printf("INVALID OPERATION\n")
            
        }
    }
    close(server_socket);
    return 0;

}
