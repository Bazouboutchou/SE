#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
void* threadA(){
	char *mess;
	mess = malloc(23);
    strcpy(mess,"Je suis A et j'ai fini\n");
	printf ("%s", mess);
	pthread_exit(mess);	
}

void* threadB(){
	char *mess;
	mess = malloc(23);
    strcpy(mess,"Je suis B et j'ai fini\n");
	fprintf(stderr, "%s","Je suis B et j'ai fini\n");	
	pthread_exit(mess);	

}

int main (){
	pthread_t threada;
	pthread_t threadb;
	char *messA;
	char *messB;
	char *mess = malloc(50);
	strcpy(mess, "Main : ");
    if(pthread_create(&threada, NULL, threadA, NULL) == -1) {
		perror("pthread_create");
		return -1;
    }
    if(pthread_create(&threadb, NULL, threadB, NULL) == -1) {
		perror("pthread_create");
		return -1;
    }
	pthread_join(threadb, (void **)&messB);
	printf("Main : %s", messB);
	pthread_join(threada, (void **)&messA);
	fprintf(stderr, "%s",strcat(mess, messA));		
}
