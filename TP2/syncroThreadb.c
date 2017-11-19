#include <stdio.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

void* threadA(void * i){
	printf("Thread A pair : %d \n", (int)i);
}

void* threadB(void * i){	
	printf("Thread B impair : %d\n", (int)i);
}

int main (){
	pthread_t threada;
	pthread_t threadb;
	int i;
	for (i = 0; i <= 1000; ++i){
		sleep(1);
		if (i % 2 == 0){
			if(pthread_create(&threada, NULL, threadA, (void*)i) == -1) {
				perror("pthread_create");
				return -1;
			}

		} else  {
			if(pthread_create(&threadb, NULL, threadB, (void*)i) == -1) {
				perror("pthread_create");
				return -1;
			}		
		}	
		
	}
	pthread_join(threada, NULL);

	pthread_join(threadb, NULL);
}
