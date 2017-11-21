#include <stdio.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

sem_t mutex;

void* threadA(){
	int i;	
	int cpt = 0;
  sem_wait(&mutex);
  
	for (i = 0; i <= 1000; ++i){

		if (i % 2 == 0){
			printf("Thread A affichage pair : ");

			printf(" %d ", i);
			printf("\n");
			++cpt;
		}
		if(cpt == 10){
			printf("\n");
			
  		sem_post(&mutex);
			sleep(1); 		
			sem_post(&mutex);	
		
			cpt = 0; 
		}
			
	}
}

void* threadB(){
	int i;	
  sem_wait(&mutex);
	int cpt = 0;

	for (i = 0; i <= 1000; ++i){
	
		if (i % 2 != 0){
			printf("Thread B affichage impair : ");

			printf(" %d ", i);
			printf("\n");
			++cpt;
		}
		if(cpt == 10){
			printf("\n");
  		sem_post(&mutex);
			sleep(1); 		
			sem_post(&mutex);	
			cpt = 0; 
		}
	}

}

int main (){
	pthread_t threada;
	pthread_t threadb;
	sem_init(&mutex, 0, 1);    
	
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
