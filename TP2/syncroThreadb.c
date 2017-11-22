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

int* partage; // donné partagé entre les threads

void* threadA(){
	do{
		sleep(1);

		if (*partage % 2 == 0) // si la donnée est pair
			printf("Thread A pair : %d \n", *partage);
	}while (*partage != 1000);

}

void* threadB(){	
	do{
		sleep(1);

		if (*partage % 2 != 0) // si la donnée est impair
			printf("Thread B impair : %d\n", *partage);
	}while (*partage != 1000);

}

int main (){
	pthread_t threada;
	pthread_t threadb;
	int fd; 
	int i;	

	fd = shm_open("partage.mem", O_RDWR | O_CREAT, 0600); // creer un segment memoire en lecture ecriture 
	if (fd == -1){
		perror("shm_open");
		exit(1);	
	}
	if ((ftruncate(fd, 4096)) == -1){ //  tronquent le fichier référencé par path ou par le descripteur fd à une longueur d'exactement length octets.


		perror("ftruncate failure");
		exit(0);
	}
	
	//  crée une nouvelle projection dans l'espace d'adressage virtuel du processus appelant. la vaiable partage est mappé en memoire protegé en lecture et ecriture 
	partage = (int*) mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if (partage == MAP_FAILED){
		perror("mmap");
		exit(1);	
	}	
	// creer le thread pair
	if(pthread_create(&threada, NULL, threadA, NULL) == -1) {
		perror("pthread_create");
		return -1;
	}
	
		// creer le thread impair
	if(pthread_create(&threadb, NULL, threadB, NULL) == -1) {
		perror("pthread_create");
		return -1;
	}		
	
	// ecrit i dans la memoire partagé
	for(i = 0; i <= 1000; ++i){
		memcpy(partage, &i, sizeof(int));
		sleep(1);
	}
	
	// attend les threads
	pthread_join(threada, NULL);

	pthread_join(threadb, NULL);
}
