#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <pthread.h> 
#include <semaphore.h>  
#define MAX_CUSTOMERS 30 

void *customer(void *num); 
void *barber(void *); 
void rwait(int secs);

sem_t waitingRoom; sem_t barberChair; sem_t barberP; sem_t seatBelt;
int all = 0;
int main(int argc, char *argv[]) 
{
 pthread_t btid; 
 pthread_t tid[MAX_CUSTOMERS]; 
 int i, x, nCustomers, nChairs; int Number[MAX_CUSTOMERS]; 
 printf("Enter number of customers and chairs.max=30\n");
 scanf("%d",&x);
 nCustomers = x; 
 scanf("%d",&x);
 nChairs = x;
 if (nCustomers > MAX_CUSTOMERS) { 
 printf("The max Customers is %d.\n", MAX_CUSTOMERS); 
 system("PAUSE"); 
 return 0;
 } 
  
 for (i = 0; i < MAX_CUSTOMERS; i++) { 
 Number[i] = i; 
 } 

 sem_init(&waitingRoom, 0, nChairs); 
 sem_init(&barberChair, 0, 1);
 sem_init(&barberP, 0, 0); 
 sem_init(&seatBelt, 0, 0); 
 

 pthread_create(&btid, NULL, barber, NULL); 
 
 
 for (i = 0; i < nCustomers; i++) {
 pthread_create(&tid[i], NULL, customer, (void *)&Number[i]); 
 } 
 
 for (i = 0; i < nCustomers; i++) { 
 pthread_join(tid[i],NULL); 
 } 
 
 all = 1; 
 sem_post(&barberP);  
 pthread_join(btid,NULL); 
 system("PAUSE"); 
 return 0;
}


void *barber(void *junk) 
{ 

 
 while (!all) { 
 printf("The barber is sleeping\n"); 
 sem_wait(&barberP); 
 if (!all) 
 { 
 printf("The barber is cutting hair\n"); 
 rwait(3);
 printf("The barber has finished cutting hair.\n"); 
 sem_post(&seatBelt); 
 } 
 else { 
 printf("The barber is done job\n"); 
 } 
 }
}
void *customer(void *number) { 
 int num = *(int *)number;
 printf("Customer %d leaving for barber shop.\n", num); 
 rwait(5); 
 printf("Customer %d arrived at barber shop.\n", num);
 sem_wait(&waitingRoom); 
 printf("Customer %d entering waiting room.\n", num);
 sem_wait(&barberChair); 
 sem_post(&waitingRoom); 
 printf("Customer %d waking the barber.\n", num); 
 sem_post(&barberP);
 sem_wait(&seatBelt); 
 sem_post(&barberChair); 
 printf("Customer %d leaving barber shop.\n", num); 
}

 
void rwait(int secs) { 
 int len = 1; 
 sleep(len); 
}
