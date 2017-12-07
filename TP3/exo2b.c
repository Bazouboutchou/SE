#include <stdio.h>
#include <fcntl.h>           /* Pour les constantes O_* */
#include <sys/stat.h>        /* Pour les constantes des modes */
#include <semaphore.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <time.h>

sem_t *places;

sem_t *arret;

sem_t *depart;


typedef struct partage
{
   int achatTicket;
   int finJournee;       /* name of the file */

} partage_struct;
//int* partage; // donné partagé entre les processus, elle me sert pour l'attente active
//int* finJournnee;

partage_struct* monPartage;

void touristes(int i){
	srand(i); // initialisation de rand avec i sinon nous auront le meme rand pour chaque

	time_t departTime, arrivee;
	int nbPlacesRestantes;
	int enterTime = rand() % 500; // rand entre 0 et 500	
	sleep (enterTime); // les touristes n'arrive pas en même temps 
	long long int diffTime;
 	time(&departTime); // moment ou je commence le chrono
	//enterTime = rand() % 5;	
	//sleep (enterTime); // simulation du temps pour que les touristes prennent un ticket
	do{	
		sem_getvalue(places, &nbPlacesRestantes); // pour tester si il y a encore des places
		time(&arrivee); // moment ou je stop le chrono
		diffTime = arrivee - departTime; // calcul le temps mis
		//si le touriste attend plus de 25 min il renonce 5s = 25min
		if (diffTime > 5 || monPartage->finJournee){
		//  printf("le touriste %d renonce \n", i);
			exit(1);		
		}
	} while(!nbPlacesRestantes || !monPartage->achatTicket); // temps qu'il n'y a pas de places dispo ou que la caisse ne soit pas ouverte
	//printf("le touriste  %d a prit un ticket\n", i);

	sem_wait(places); // si le temps d'attente est < 25 le touriste prend une place
	exit(1);
				
}

void chauffeur(){

	int i;
	int nbTouristes; 
	do{	

		sem_wait(depart); // on attend que la caisse finnisse 
		
		sleep(1); // pause de 5 min

		sem_getvalue(places, &nbTouristes); // recupere le nombre de place prisent
		nbTouristes = 20 - nbTouristes; // calcul le nombre de touristes dans le bus		
		printf("Chauffeur klaxone \n");
		sem_post(arret); // le caissier recommence
		
		for(i = 0; i < nbTouristes; ++i){
			sem_post(places); // les touristes sortent	
		}

		printf("Nombre de personnes embarquées : %d \n", nbTouristes);
		sleep(4); // 20 min de transport
		printf("Arrive du chauffeur \n");

	}while (!monPartage->finJournee); // temps que l'on a pas realiser tous les cycles de 50 min
	exit(1);
}

void caissier(){
	printf("Ouverture de la caisse 8h45\n");
	int duree = 0;
	int tempsTotalMinutes = 45;
	int tempsTotalHeures = 8; 
	int nbPlacesRestantes;	
	int fini = 0;
	time_t departTime, arrivee;
	long long int diffTime;
 	time(&departTime); // moment ou je commence le chrono
	
	do{
		
		sem_getvalue(places, &nbPlacesRestantes); // pour tester si il y a encore des places
		time(&arrivee);
		diffTime = arrivee - departTime;
		
		if ((diffTime >= 5 || !nbPlacesRestantes) || (tempsTotalHeures >= 16 && tempsTotalMinutes >= 30)){
			// calcul de l'heure
			if (tempsTotalMinutes + diffTime*5 >= 60)
				++tempsTotalHeures;		
			tempsTotalMinutes = (tempsTotalMinutes + diffTime*5)%60; // 25 + 5 +20 min de pause du chauffeur + 20 min de trajet

			// le bus commence seulement à 9h
			if (nbPlacesRestantes == 0 && tempsTotalHeures < 9){
				sleep(60-tempsTotalMinutes); // on attend le nombre de min restantes avant 9h
				tempsTotalHeures = 9;
				tempsTotalMinutes = 0;
			}

			monPartage->achatTicket = 0;
			printf("Caissier ferme la vente à : %dh%d \n", tempsTotalHeures, tempsTotalMinutes);	
			sem_post(depart);// on dit au bus de partir
			sem_wait(arret); // on arrete la vente
			if (tempsTotalMinutes + 5 >= 60)
				++tempsTotalHeures;		
			tempsTotalMinutes = (tempsTotalMinutes + 5)%60; // 25 + 5 +20 min de pause du chauffeur + 20 min de trajet
			printf("Caissier reouvre la vente à : %dh%d \n", tempsTotalHeures, tempsTotalMinutes);	

			monPartage->achatTicket = 1;   // on reprend la vente			
		 	time(&departTime); // moment ou je commence le chrono
			if(tempsTotalHeures >= 16 && tempsTotalMinutes >= 30){
				monPartage->finJournee = 1;
			}

		}// si le caissier a attendu 25 min il part 		

	}while (!fini); // temps que l'on a pas realiser tous les cycles de 50 min
	
	printf("Arret de la vente 16h30\n");
	exit(1);
}


int main (){
	int fd; 
	int i;	
	
	fd = shm_open("partage.mem", O_RDWR | O_CREAT, 0644); // creer un segment memoire en lecture ecriture 
	if (fd == -1){
		perror("shm_open");
		exit(1);	
	}
	if ((ftruncate(fd, 4096)) == -1){ //  tronquent le fichier référencé par path ou par le descripteur fd à une longueur d'exactement length octets.


		perror("ftruncate failure");
		exit(0);
	}
	
	//  crée une nouvelle projection dans l'espace d'adressage virtuel du processus appelant. la vaiable partage est mappé en memoire protegé en lecture et ecriture 
	monPartage = (partage_struct*) mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if (monPartage == MAP_FAILED){
		perror("mmap");
		exit(1);	
	}	
	sem_unlink ("places");
	sem_unlink ("arret"); 	
	sem_unlink ("depart");
	
	monPartage->achatTicket = 1;
	monPartage->finJournee = 0;
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

	for (i = 1; i < 200; ++i){
		if (!fork()){
			touristes(i);
		}
	}	

	wait(NULL);
	wait(NULL);
	wait(NULL);	
	for (i 	= 1; i < 200; ++i){
		wait(NULL);
	}	
	sem_unlink ("places");
	sem_unlink ("arret"); 	
	sem_unlink ("depart");
}
