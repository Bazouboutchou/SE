#include <stdio.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h> 

#define NO_PLACES  3
#define NO_CARS    5
#define NO_ENTRIES 2
#define NO_EXITS   2 

sem_t places; 
sem_t entrees;
sem_t sorties;
pthread_cond_t condNoPlace = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex ;//= PTHREAD_MUTEX_INITIALIZER;

void entrer(int carId){
	printf("Voiture %d arrive dans lentree \n", carId);
	pthread_mutex_unlock(&mutex);
	printf("Voiture %d sortie entree \n", carId);
	pthread_mutex_lock(&mutex);
}

void sortie(int carId){
	printf("Voiture %d arrive dans la sortie \n", carId);
	sem_wait(&sorties);
	sleep(1);
	printf("Voiture %d sortie de sortie \n", carId);
	sem_post(&sorties);
}

void leaveParking(int carId){
	sem_post(&places);
	pthread_cond_signal(&condNoPlace);
	printf("Voiture %d est sortie \n", carId);
}

void enterParking(int carId){
	int isWaiting;
	printf("Voiture %d arrive dans le parking \n", carId);	
	sem_getvalue(&places, &isWaiting);	
	if (!isWaiting){
		printf("Voiture bloque à l entree\n");	
		pthread_cond_wait(&condNoPlace, &mutex);	
	}	// si plus de place on bouche les entrees
	sem_wait(&places);
	printf("Voiture %d a trouve une place \n", carId);
}


void* carLife(void * carId){
	int enterTime = rand() % 10;
	int leaveTime = rand() % 10;
//	printf("Voiture %d attend %d s avant d'entrer\n", (int) carId, enterTime);
	sleep(enterTime);
	entrer((int)carId);
	enterParking((int)carId);
//	printf("Voiture %d attend %d s avant de sortir\n", (int) carId, leaveTime);
	sleep(leaveTime);
	leaveParking((int)carId);
	sortie((int)carId);
}


int main(){
	int i;
 	srand(time(NULL)); // initialisation de rand	
	pthread_t threadCars[NO_CARS];
	
	sem_init(&places,  0, NO_PLACES);  		
	sem_init(&entrees, 0, NO_ENTRIES);  		
	sem_init(&sorties, 0, NO_EXITS);  		
	pthread_mutex_init(&mutex, NULL);
	for (i = 0; i < NO_CARS; ++i){
	 	if(pthread_create(&threadCars[i], NULL, carLife, (void*)i) == -1) {
			perror("pthread_create");
			return -1;
		}

	}
	for (i = 0; i < NO_CARS; ++i){		
		pthread_join(threadCars[i], NULL);
	}
}

