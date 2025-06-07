#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

int nr,np, work[10]; 
int avail[10],alloc[10][10];
bool finish[10];
int request[10][10];

void deadLockDetectionAlgorithm(){
 
    for(int i =0;i< nr ; i++)
        work[i]= avail[i];
    
    for(int i =0 ; i < np ; i++)
        finish[i] = false;

    bool possible = true;
    for(int i =0 ; i< 5 ; i++ )
        for(int  i =0; i< np ; i++){
            possible = true;
            if(!finish[i]){
                for(int j = 0 ;j < nr; j++)
                    if(request[i][j] > work[j] ){
                        possible=false;
                        break;
                    }             
                if(possible){
                    for(int k = 0 ;k < nr; k++)
                        work[k]+= alloc[i][k];
                    finish[i] = true;
             
                }
            }
        }

    possible = true;
    for(int i =0;i< np ;i++)
        if(!finish[i]){
            possible = false;
            printf("THE SYSTEM IS IN A DEADLOCKED STATE");
            break;
        }
    if(possible)
        printf("THERE IS NO DEADLOCK IN THE GIVEN SYSTEM");    
    
}

int main(){
    printf("ENTER THE NUMBER OF RESOURCES: ");
    scanf("%d",&nr);
    printf("ENTER THE AVAILABLE RESOURCES: ");
    for(int i =0;i< nr ; i++){
        scanf("%d",&avail[i]);
        work[i]= avail[i];
    }
    printf("ENTER THE NUMBER OF PROCESSES : ");
    scanf("%d",&np);
    printf("ENTER THE ALLOCATION MATRIX\n");
    for(int i = 0;i< np; i++)
        for(int j =0; j< nr;j++)
            scanf("%d",&alloc[i][j]);
    printf("ENTER THE REQUEST MATRIX \n");
    for(int i = 0;i< np; i++)
        for(int j =0; j< nr;j++)
            scanf("%d",&request[i][j]);
    deadLockDetectionAlgorithm();
 
}