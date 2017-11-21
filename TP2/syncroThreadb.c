// https://www.blaess.fr/christophe/2011/10/09/efficacite-des-ipc-semaphore-et-memoire-partagee/
#include <stdio.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
int i;
void* threadA(void * i){
	printf("Thread A pair : %d \n", (int)i);
}

void* threadB(void * i){	
	printf("Thread B impair : %d\n", (int)i);
}

int main (){
	pthread_t threada;
	pthread_t threadb;
	int fd; 
	char *partage NULL;
	int i;	
	fd = shm_open("partage.mem", O_RDWR | O_CREAT, 0600);
	if (fd == -1){
		perror("shm_open");
		exit(1);	
	}
	partage = (char*) mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if (partage == MAP_FAILED){
		perror("mmap");
		exit(1);	
	}	
	
	while(i(i = 0; i <= 1000; ++i){
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
