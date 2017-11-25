#include <stdio.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

sem_t mutexA;
sem_t mutexB;

void* threadA(){
	int i;	
	int cpt = 0;
	sem_wait(&mutexA);	
 
	for (i = 1; i <= 1000; ++i){
 		
		if (i % 2 == 0){
			printf("Thread A affichage pair : ");

			printf(" %d ", i);
			printf("\n");
			++cpt;
		}
		if(cpt == 10){
			printf("\n");		
			sem_post(&mutexB);	
			sem_wait(&mutexA);

			cpt = 0; 
		}
				
	}
}

void* threadB(){
	int i;	

	int cpt = 0;
 	//sem_wait(&mutexB);	

	for (i = 1; i <= 1000; ++i){
		if (i % 2 != 0){
			printf("Thread B affichage impair : ");

			printf(" %d ", i);
			printf("\n");
			++cpt;
		}
		if(cpt == 10){
			printf("\n");
			sem_post(&mutexA);	
	  	sem_wait(&mutexB);	

			cpt = 0; 
		}
	}
	sem_post(&mutexA);
}

int main (){
	pthread_t threada;
	pthread_t threadb;
	sem_init(&mutexA, 0, 0);    
	sem_init(&mutexB, 0, 0);    
	
  if(pthread_create(&threadb, NULL, threadB, NULL) == -1) {
		perror("pthread_create");
		return -1;
  }
	if(pthread_create(&threada, NULL, threadA, NULL) == -1) {
		perror("pthread_create");
		return -1;
  }
	pthread_join(threadb, NULL);
	pthread_join(threada, NULL);
}
