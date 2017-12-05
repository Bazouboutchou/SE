// https://ensiwiki.ensimag.fr/images/8/85/Pb_philosophes.pdf
// https://sites.uclouvain.be/SystInfo/notes/Theorie/html/Threads/coordination.html
#include <stdio.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h> 

#define NO_PLACES 2
#define NO_CARS   3


sem_t places;

/*
 * @param : entier qui est l identifiant de la voiture
 * @brief : decremente le semaphore, ce qui represente au fait 
 *          qu'une voiture a prit une place disponible.
 *					j'ai changer le nom de la fonction, puique sémantiquement
 *          elle correspond plutot à une prise de place qu'au fait 
 *          qu'une voiture rentre dans le parking
 */

void prendrePlace(int carId){
	printf("Voiture %d arrive dans le parking \n", carId);
	sem_wait(&places);
	printf("Voiture %d a trouve une place \n", carId);
}

/*
 * @param : entier qui est l identifiant de la voiture
 * @brief : increment le semaphore, ce qui represente au fait 
 *          qu'une voiture a libere une place.
 *					j'ai changer le nom de la fonction, puique sémantiquement
 *          elle correspond plutot à la liberation d'une place et non à une 
 * 				  voiture qui sort du parking 
 */
void quitterPlace(int carId){
	sem_post(&places);
	printf("Voiture %d a quitter la place \n", carId);
}

/*
 * @param : entier qui est l identifiant de la voiture
 * @brief : Cycle de vie d'une voiture. J'ai placé des sleep avec valeur 
 *          aleatoire (les voitures n'arrivent pas ensemble, et reste un
 *          certain temps dans le parking). En soit dans cette fonction : 
 *          la voiture arrive, entre dans le parking prend une place 
 *          si il y en a une disponible, sinon elle attend. Puis elle 
 *          attend un temps aléatoire et quitte le parking
 */
void* carLife(void * carId){
	int enterTime = rand() % 10;
	int leaveTime = rand() % 10;
	printf("Voiture %d attend %d s avant d'entrer\n", (int) carId, enterTime);
	sleep(enterTime);
	prendrePlace((int)carId);
	printf("Voiture %d attend %d s avant de sortir\n", (int) carId, leaveTime);
	sleep(leaveTime);
	quitterPlace((int)carId);
}


int main(){
	int i;
 	srand(time(NULL)); // initialisation de rand	
	pthread_t threadCars[NO_CARS]; // autant de thread que de voiture
	
	sem_init(&places, 0, NO_PLACES); // notre semaphoreest initialisé au nombre de place disponible

	for (i = 0; i < NO_CARS; ++i){
	 	if(pthread_create(&threadCars[i], NULL, carLife, (void*)i) == -1) {
			perror("pthread_create");
			return -1;
		} 

	} // création de tout les threads
	for (i = 0; i < NO_CARS; ++i){		
		pthread_join(threadCars[i], NULL);
	} // on attend qu'ils finnissent tous
}


/* trace 
Voiture 4 attend 5 s avant d'entrer
Voiture 3 attend 1 s avant d'entrer
Voiture 2 attend 3 s avant d'entrer
Voiture 1 attend 7 s avant d'entrer
Voiture 0 attend 4 s avant d'entrer
Voiture 3 arrive dans le parking 
Voiture 3 a trouve une place 
Voiture 3 attend 3 s avant de sortir
Voiture 2 arrive dans le parking 
Voiture 2 a trouve une place 
Voiture 2 attend 7 s avant de sortir
Voiture 0 arrive dans le parking 
Voiture 0 a trouve une place 
Voiture 0 attend 9 s avant de sortir
Voiture 3 est sortie 
Voiture 4 arrive dans le parking 
Voiture 4 a trouve une place 
Voiture 4 attend 5 s avant de sortir
Voiture 1 arrive dans le parking 
Voiture 2 est sortie 
Voiture 1 a trouve une place 
Voiture 1 attend 7 s avant de sortir
Voiture 4 est sortie 
Voiture 0 est sortie 
Voiture 1 est sortie 
*/
