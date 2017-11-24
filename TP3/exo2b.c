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

int* partage; // element dans la memoire partage pour compter le temps 

void touristes(){
	
	int enterTime = rand() % 5; // rand entre 0 et 5
	sleep (enterTime);
	if (*partage) // si les 25 mins son pass
		sem_wait(places); // si le temps d'attente est < 25 le touriste prend une place
	exit(1);			
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
		sleep(4); // 20 min de transport
		++duree;
		printf("Arrive du chauffeur \n");
		sem_post(arret); // le caissier recommence
		
		for(i = 0; i < nbTouristes; ++i){
			sem_post(places); // les touristes sortent	
		}
	}while (duree < 9);
	exit(1);
}

void caissier(){
	printf("Ouverture de la caisse 8h45\n");
	sleep(1);		
	int duree = 0;
	int tempsTotalMinutes = 45;
	int tempsTotalHeures = 8; 
	
	do{
		
		sleep(5); // attend 25 min de partir
		*partage = 0;	
		// depart 
		sem_post(depart);
		sem_wait(arret);
		if (tempsTotalMinutes + 50 >= 60)
			++tempsTotalHeures;		
		tempsTotalMinutes = (tempsTotalMinutes + 50)%60; // 25 + 5 +20 min de pause du chauffeur + 20 min de trajet
		

		printf("Caissier il est : %dh%d \n", tempsTotalHeures, tempsTotalMinutes);	
		++duree;
		*partage = 1;
	}while (duree < 9); // nombre d'iteration jusqua 16h30
	
	printf("Arret de la vente 16h30\n");
	exit(1);
}


int main (){
	int i;
	int fd;
 	srand(time(NULL)); // initialisation de rand		
	sem_unlink ("places");
	sem_unlink ("arret"); 	
	sem_unlink ("depart");

	places = sem_open ("places", O_CREAT|O_EXCL, 0644, 20);
	arret  = sem_open ("arret" , O_CREAT|O_EXCL, 0644, 0);
	depart = sem_open ("depart", O_CREAT|O_EXCL, 0644, 0);
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
	partage = (int*) mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if (partage == MAP_FAILED){
		perror("mmap");
		exit(1);	
	}		
	memcpy(partage, 0, sizeof(int));
	if (!fork()){
		caissier();
		exit(1);
	}

	if (!fork()){
		chauffeur();
		exit(1);
	}


	for (i 	= 0; i < 200; ++i){
		if (!fork()){
			touristes();
		}
	}	

	wait(NULL);
	sem_unlink ("places");
	sem_unlink ("arret"); 	
	sem_unlink ("depart");
}
