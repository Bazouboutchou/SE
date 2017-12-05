#include <stdio.h>
#include <fcntl.h>           /* Pour les constantes O_* */
#include <sys/stat.h>        /* Pour les constantes des modes */
#include <semaphore.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

sem_t *places;

sem_t *arret;

sem_t *depart;

void touristes(){
	
	int enterTime = rand() % 30; // rand entre 0 et 35
//	sleep (1);
	//if (enterTime > 25)
		sem_wait(places); // si le temps d'attente est < 25 le touriste prend une place
			
}

void chauffeur(){

	int duree = 0;	
	int i;
	int nbTouristes; 
	do{	

		sem_wait(depart);
		sleep(1); // pause de 5 min
		sem_getvalue(places, &nbTouristes);
		nbTouristes = 20 - nbTouristes; // calcul le nombre de touristes dans le bus		
		printf("Chauffeur klaxone \n");
		printf("Nombre de personnes embarquées : %d \n", nbTouristes);
		sleep(2);
		++duree;
		printf("Arrive du chauffeur \n");
		sem_post(arret); // le caissier recommence
		
		for(i = 0; i < nbTouristes; ++i){
			sem_post(places); // les touristes sortent	
		}
	}while (duree < 9);
}

void caissier(){
	printf("Ouverture de la caisse 8h45\n");		
	int duree = 0;
	int tempsTotalMinutes = 45;
	int tempsTotalHeures = 8; 
	
	do{
		
		sleep(3); // attend 25 min de partir
				
		// depart 
		sem_post(depart);
		sem_wait(arret);
		if (tempsTotalMinutes + 50 >= 60)
			++tempsTotalHeures;		
		tempsTotalMinutes = (tempsTotalMinutes + 50)%60; // 25 + 5 +20 min de pause du chauffeur + 20 min de trajet
		

		printf("Caissier il est : %dh%d \n", tempsTotalHeures, tempsTotalMinutes);	
		++duree;
		
	}while (duree < 9); // nombre d'iteration jusqua 16h30
	
	printf("Arret de la vente 16h30\n");

}


int main (){
	int i;
 	srand(time(NULL)); // initialisation de rand		
	sem_unlink ("places");
	sem_unlink ("arret"); 	
	sem_unlink ("depart");

	places = sem_open ("places", O_CREAT|O_EXCL, 0644, 20);
	arret  = sem_open ("arret" , O_CREAT|O_EXCL, 0644, 0);
	depart = sem_open ("depart", O_CREAT|O_EXCL, 0644, 0);
	
		
	if (!fork()){
		caissier();
		exit(1);
	}

	if (!fork()){
		chauffeur();
		exit(1);
	}


	for (i = 0; i < 200; ++i){
		if (!fork()){
			touristes();
			exit(1);
		}
	}	

	wait(NULL);
	sem_unlink ("places");
	sem_unlink ("arret"); 	
	sem_unlink ("depart");
}
