//Including all necessary header files
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t lockResources;
pthread_cond_t condition;

//Process Function declaration
void *PCode(void *);


int nR, nP;
int Prorun = 0;

//allocation of 2D and 1D arrays for process and resources
int alloc[100][100];                                           //for allocation
int max[100][100];                                             //for max
int need[100][100];                                            //for need of process
int avail[100];                                                    //for available resources
int flag[100];                                                       //for flagging processes
int seq[100];

int main()      //main() function
{
    int process;                                                         //for no. of process
    int resources;                                                       //for no. of resources
    printf("***************************************\n");
    printf("Program for implimenting bankers algo\n");
    printf("***************************************\n\n");
    printf("Enter the no. of process - ");
    scanf("%d",&process);
    printf("Enter the no. of resources - ");
    scanf("%d",&resources);
    nR = resources; nP = process;

    int seqc = 0;
    int i,j,k,x=0,count;

    for(i=0;i<process;i++){
        flag[i]=0;                                                      //none of the process's need is satisfied initially
    }
    for(i=0;i<process;i++){
            printf("Enter the allocation resources for process P%d (R1,R2,....) - ",i+1);
            for(j=0;j<resources;j++){
                scanf("%d",&alloc[i][j]);
            }
            printf("Enter the max resources for process P%d (R1,R2,.....) -       ",i+1);
                for(j=0;j<resources;j++){
                scanf("%d",&max[i][j]);
            }
            printf("\n");
}
    for(i=0;i<resources;i++){
        printf("Enter the available of resource R%d - ", i+1);
        scanf("%d",&avail[i]);
    }
    for(i=0;i<process;i++){
        for(j=0;j<resources;j++){
            need[i][j]=max[i][j]-alloc[i][j];                            
        }
    }

    for(i=0;i<process;i++){ //loop to print the calculated need
        printf("Need of P%d is:",i+1);
        for(j=0;j<resources;j++){
            printf("%d ",need[i][j]);
        }
        printf("\n");
    }

    for(i=0;i<process;i++){ //loop for iteration
        for(j=0;j<process;j++){ //loop for process
            count=0;
            for(k=0;k<resources;k++){ //loop for resource
                if(flag[j]==0&&need[j][k]<=avail[k]){   
                    avail[k] = avail[k] + alloc[j][k];
                    count++;
                    if(count == resources){                        
                        flag[j] = 1;                                    
                        printf("Process %d has been allocated resources\n",j+1);
                        seq[seqc] = j;                                 
                        seqc++;
                    }
                    continue;                                           
                }
                break;                                                  
            }
        }
    }
    for(i=0;i<process;i++){
        if(flag[i]==0){
            printf("Unsafe state found in system\n");
            printf("Aborting!!!!\n");
            x=1;
            break;
        }
    }
    if(x==0){
        printf("System is in safe state\n");
        printf("Safe sequence found is \n");
        for(i=0;i<process;i++){
            printf("P[%d],",seq[i]);
        }
    }
    printf("\n");

    printf("Executing process!!!!!!.......");

    pthread_t threads[process];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

	int processNumber[process];

	for(int i=0; i<process; i++){
            processNumber[i] = i;
    }
    for(int i=0; i<process; i++){
                pthread_create(&threads[i], &attr, PCode, (void *)(&processNumber[i]));
    }
    for(int i=0; i<process; i++){
                pthread_join(threads[i], NULL);
    }
    printf("\nAll Processes Finished\n");

    return 0;
}

void *PCode(void * args){
    int p = *((int *) args);
    pthread_mutex_lock(&lockResources);
    // condition check
    while(p != seq[Prorun])
            pthread_cond_wait(&condition, &lockResources);
	// process
	printf("****************************");
    printf("   Process Running P%d\n", p+1);
    printf("****************************");
    printf("Allocated : ");
    for(int i=0; i<nR; i++)
            printf("%d ", alloc[p][i]);
    printf("\n\tNeeded    : ");
    for(int i=0; i<nR; i++)
            printf("%d ", need[p][i]);
    printf("\n\tAvailable : ");
    for(int i=0; i<nR; i++)
            printf("%d ", avail[i]);
    printf("Thread Enter critical section \n");
    sleep(rand()%3 + 2); // process code
    printf("Process Code Completed...\n");

	for(int i=0; i<nR; i++)
            avail[i] += alloc[p][i];
    printf("\nChanged Available - ");
    for(int i=0; i<nR; i++)
            printf("%d ", avail[i]);
    printf("\n");
    printf("****************************\n\n");

	// condition broadcast
    Prorun++;
    pthread_cond_broadcast(&condition);
    pthread_mutex_unlock(&lockResources);
    pthread_exit(NULL);
}

