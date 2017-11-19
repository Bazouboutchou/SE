#include <stdio.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
pthread_mutex_t verrou; 
void* threadA(){
	int i;	
	printf("Thread A affichage pair :\n");
	int cpt = 0;

	for (i = 0; i <= 1000; ++i){
		pthread_mutex_unlock(&verrou);
		if (i % 2 == 0){
			printf(" %d ", i);
			++cpt;
		}
		if(cpt == 10){
			printf("\n");
			
			pthread_mutex_lock(&verrou);
		
			cpt = 0; 
		}
			
	}
}

void* threadB(){
	int i;	
	printf("Thread B affichage impair :\n");

	int cpt = 0;

	for (i = 0; i <= 1000; ++i){
		pthread_mutex_lock(&verrou);

		if (i % 2 != 0){
			printf(" %d ", i);
			++cpt;
		}
		if(cpt == 10){
			printf("\n");
			pthread_mutex_unlock(&verrou);
			cpt = 0; 
		}
	}

}

int main (){
	pthread_t threada;
	pthread_t threadb;
	pthread_mutex_init(&verrou, NULL); 
    if(pthread_create(&threada, NULL, threadA, NULL) == -1) {
		perror("pthread_create");
		return -1;
    }

    if(pthread_create(&threadb, NULL, threadB, NULL) == -1) {
		perror("pthread_create");
		return -1;
    }
	pthread_join(threada, NULL);
	pthread_join(threadb, NULL);
}
