#include <stdio.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

void* threadA(){
	int i;	
	printf("Thread A affichage pair :\n");

	for (i = 0; i <= 1000; ++i){
		if (i % 2 == 0)		
			printf(" %d ", i);
	}
}

void* threadB(){
	int i;	
	printf("Thread B affichage impair :\n");

	for (i = 0; i <= 1000; ++i){
		if (i % 2 != 0)		
			printf(" %d ", i);
	}

}

int main (){
	pthread_t threada;
	pthread_t threadb;
    if(pthread_create(&threada, NULL, threadA, NULL) == -1) {
		perror("pthread_create");
		return -1;
    }
	pthread_join(threada, NULL);

    if(pthread_create(&threadb, NULL, threadB, NULL) == -1) {
		perror("pthread_create");
		return -1;
    }
	pthread_join(threadb, NULL);
}
