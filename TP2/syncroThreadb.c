// https://www.blaess.fr/christophe/2011/10/09/efficacite-des-ipc-semaphore-et-memoire-partagee/
#include <stdio.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <sys/mman.h>
#include <string.h>

int* partage;

void* threadA(){
	do{
		sleep(1);

		if (*partage % 2 == 0)
			printf("Thread A pair : %d \n", *partage);
	}while (*partage != 1000);

}

void* threadB(){	
	do{
		sleep(1);

		if (*partage % 2 != 0)
			printf("Thread B impair : %d\n", *partage);
	}while (*partage != 1000);

}

int main (){
	pthread_t threada;
	pthread_t threadb;
	int fd; 
	int i;	

	fd = shm_open("partage.mem", O_RDWR | O_CREAT, 0600);
	if (fd == -1){
		perror("shm_open");
		exit(1);	
	}
	if ((ftruncate(fd, 4096)) == -1){
		perror("ftruncate failure");
		exit(0);
	}

	partage = (int*) mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if (partage == MAP_FAILED){
		perror("mmap");
		exit(1);	
	}	
	if(pthread_create(&threada, NULL, threadA, NULL) == -1) {
		perror("pthread_create");
		return -1;
	}
	if(pthread_create(&threadb, NULL, threadB, NULL) == -1) {
		perror("pthread_create");
		return -1;
	}		

	for(i = 0; i <= 1000; ++i){
		memcpy(partage, &i, sizeof(int));
		sleep(1);
	}
	pthread_join(threada, NULL);

	pthread_join(threadb, NULL);
}
