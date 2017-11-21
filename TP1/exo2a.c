#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/*
 * @brief : thread qui affiche un message dans la sortie 
 *          standard, puis renvoi une chaine 
 *          de caractere comportant cette chaine
 */
void* threadA(){
	char *mess;
	mess = malloc(23); // 23 = taille de la chaine
  strcpy(mess,"Je suis A et j'ai fini\n"); // place la chaine dans une variable
	printf ("%s", mess);
	pthread_exit(mess);	// permet de recuperer la chaine dans le main thread
}

/*
 * @brief : thread qui affiche un message dans la sortie 
 *          derreur, puis renvoi une chaine 
 *          de caractere comportant cette chaine
 */
void* threadB(){
	char *mess;
	mess = malloc(23); // 23 = taille de la chaine
  strcpy(mess,"Je suis B et j'ai fini\n"); // place la chaine dans une variable
	fprintf(stderr, "%s","Je suis B et j'ai fini\n"); // affichage dans la sortie derreur	
	pthread_exit(mess);	// permet de recuperer la chaine dans le main thread

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
  } // appel du 1er thread
  if(pthread_create(&threadb, NULL, threadB, NULL) == -1) {
		perror("pthread_create");
		return -1;
  }// appel du 2nd thread
	pthread_join(threadb, (void **)&messB); // attend threadB
	printf("Main : %s", messB);
	pthread_join(threada, (void **)&messA); // attend threadA
	fprintf(stderr, "%s",strcat(mess, messA));		
}



/* trace : 
Je suis B et j'ai fini
Main : Je suis B et j'ai fini
Je suis A et j'ai fini
Main : Je suis A et j'ai fini
*/
