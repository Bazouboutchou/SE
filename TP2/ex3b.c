#include <stdio.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h> 

#define NO_PLACES  1
#define NO_CARS    3
#define NO_ENTRIES 2
#define NO_EXITS   2 

sem_t places; 
sem_t entrees;
sem_t sorties;

int nbCarEntree = 0; // correspond aux nombres de voiture dans le parking
	// Ce compte me permet de bloquer les voitures dans l'entree si il n'y 
  // a plus de place disponnible


/*
 * @param : entier qui est l identifiant de la voiture
 * @brief : cette fonction correspond a l'entree d'une voiture.
 *          La voiture entre dans l'entree, decremente le semaphore
 *          puis des quelle sort incremente le semaphore. Cette fonction
 *          permet également d'empecher les voitures de rentrer si il 
 *          n'y a plus de place disponnible. 
 */ 
void entrer(int carId){
	++nbCarEntree; // incrementre le nombre de voiture presente		
	printf("Voiture %d fait la queue dans lentree \n", carId);
	
	sem_wait(&entrees); // on attend dans l'entree

	if (nbCarEntree > NO_PLACES){ // si il y a plus de voiture que de place
		int entree;
		do { // tant que le semaphore est different de 0
			sem_getvalue(&entrees, &entree);	// recupere la valeur du semaphore
			
			sem_wait(&entrees);		
			
		}	while (entree-1); // on fait attendre toutes les voitures dans l'entree
		
	}	
	

	sleep(1);	

	printf("Voiture %d sortie entree \n", carId);
	sem_post(&entrees); // la voiture est sortie
}


/*
 * @param : entier qui est l identifiant de la voiture
 * @brief : decremente le semaphore, ce qui represente au fait 
 *          qu'une voiture a prit une place disponible.
 *					j'ai changer le nom de la fonction, puique sémantiquement
 *          elle correspond plutot à une prise de place qu'au fait 
 *          qu'une voiture rentre dans le parking. Dans le contexte de 
 *          cette exercice, le semephore sur le nombre de place disponible 
 * 				  ne sert plus a grand chose, puisque  les voitures sont regulés
 *          dans l'entree 
 */

void prendrePlace(int carId){
	sem_wait(&places); // on attend une place
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
	sem_post(&places); // libere une place
	printf("Voiture %d libere la place \n", carId);
	if (nbCarEntree >= NO_PLACES){
		int i;		
		for (i = 0; i < NO_ENTRIES; ++i){
			sem_post(&entrees);
			--nbCarEntree;
		}		
		sleep(1);
	} // si le toutes les places étaient deja prisent, 
	// les voitures etaient bloquees dans l'entree. Ici on libere une ressource
}

/*
 * @param : entier qui est l identifiant de la voiture
 * @brief : cette fonction correspond a la sortie d'une voiture.
 *          la voiture entre dans la sortie, decremente le semaphore
 *          puis des quelle sort incremente le semaphore 
 */
void sortie(int carId){
	printf("Voiture %d arrive dans la sortie \n", carId);
	sem_wait(&sorties);
	sleep(1);
	printf("Voiture %d sortie de sortie \n", carId);
	sem_post(&sorties);
	
}

/*
 * @param : entier qui est l identifiant de la voiture
 * @brief : Cycle de vie d'une voiture. J'ai placé des sleep avec valeur 
 *          aleatoire (les voitures n'arrivent pas ensemble, et reste un
 *          certain temps dans le parking). En soit dans cette fonction : 
 *          la voiture arrive dans l'entree si il y en a une des disponible,
 *					entre dans le parking prend une place 
 *          si il y en a une de disponible, sinon elle attend. Puis elle 
 *          attend un temps aléatoire libere la place et va dans une sortie 
 *          si il y en a une de disponible
 * 
 */
void* carLife(void * carId){
	int enterTime = rand() % 10; // temps pour entrer random
	int leaveTime = rand() % 10; // temps pour sortir random
//	sleep(enterTime);
	entrer((int)carId);
	prendrePlace((int)carId);
	sleep(leaveTime);
	quitterPlace((int)carId);
	sortie((int)carId);
}


int main(){
	int i;
 	srand(time(NULL)); // initialisation de rand	
	pthread_t threadCars[NO_CARS]; // autant de threads que de voitures
	
	// initialisation de tous les semaphores avec la constante correspondante
	sem_init(&places,  0, NO_PLACES);  		
	sem_init(&entrees, 0, NO_ENTRIES);  		
	sem_init(&sorties, 0, NO_EXITS);  		
	for (i = 0; i < NO_CARS; ++i){
	 	if(pthread_create(&threadCars[i], NULL, carLife, (void*)i) == -1) {
			perror("pthread_create");
			return -1;
		}

	}// création de tout les threads
	for (i = 0; i < NO_CARS; ++i){		
		pthread_join(threadCars[i], NULL);
	} // on attend qu'ils finnissent tous
}

