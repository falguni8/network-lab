#include<stdio.h>
#include<string.h>

const int INF=100000;



int main(){
    int n,m;
    //no of nodes
    //total number of links
    scanf("%d %d",&n,&m);
    
    int adj[n][n];
    int nxt_hop[n][n];

    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(i==j){
                adj[i][j]=0;
                nxt_hop[i][j]=j;
            }
            else{
                adj[i][j]=INF;
                adj[j][i]=INF;
            }
        }
    }

    for(int i=0;i<m;i++){
        int x,y,w;
        scanf("%d %d %d",&x,&y,&w);
        printf("entered ..\n");
        adj[x-1][y-1]=adj[y-1][x-1]=w;
        nxt_hop[x-1][y-1]=y-1;
        nxt_hop[y-1][x-1]=x-1;
    }
    printf("Matrix....\n");
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            printf("%d ",adj[i][j]);
        }
        printf("\n");
    }
    printf(".................\n");
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            for(int k=0;k<n;k++){
                if(adj[i][j]>adj[i][k]+adj[k][j]){
                    adj[i][j]=adj[i][k]+adj[k][j];
                    nxt_hop[i][j]=nxt_hop[i][k];
                }
            }
        }
    }

    for(int i=0;i<n;i++){
        printf("Routing tableat node %d\n",i+1);
        printf("Dest|Cost|Next\n");
        for(int j=0;j<n;j++){
            printf("%d %d %d\n",j+1,adj[i][j],nxt_hop[i][j]+1);
        }
    }

    return 0;
}
