#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

struct account{
  int accountnum;
  float balance;
  pthread_mutex_t lockResources;
}a[2];


pthread_mutex_init(&(a[0]->lockResources), NULL);
pthread_mutex_init(&(a[1]->lockResources), NULL);

void *processCode();
void transfer(struct account *, struct account *, float);

float money;

int main(){

  for (int i=0;i < 2;i++) {
    printf("Enter Ac. number of account %d - \n", i);
    scanf("%d", &a[i]->accountnum);
    printf("Enter Ac. balance of account %d - \n", i);
    scanf("%f", &a[i]->balance);
  }

  printf("Enter the money to be transfered from AC 1 to AC 2 -\n");
  scanf("%f", &money);

  printf("Current balance in account %d - %f\n", a[0]->accountnum, a[0]->balance);
  printf("Current balance in account %d - %f\n", a[1]->accountnum, a[1]->balance);

  pthread_t func;

  pthread_create(&func, NULL, processCode, NULL);

  pthread_join(&func, NULL);

  printf("Updated balance in account %d - %f\n", a[0]->accountnum, a[0]->balance);
  printf("Updated balance in account %d - %f\n", a[1]->accountnum, a[1]->balance);

}

void* processCode(void *args) {

        pthread_mutex_lock(&a[0]->lockResources);
        pthread_mutex_lock(&a[1]->lockResources);

        transfer(a[0], a[1], money);

        pthread_mutex_unlock(&a[1]->lockResources);
        pthread_mutex_unlock(&a[0]->lockResources);
}

void transfer(struct account *from, struct account *to, float amount){
    if(from->balance < amount){
      printf("Insufficient Funds\n");
      printf("Aborting program!!!!\n");
    }else{
      from->balance -= amount;
      to->balance += amount;
    }
}
