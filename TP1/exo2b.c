//http://gkemayo.developpez.com/tempsreel/programmation-multitache-posix/
#include <stdio.h>
#include <pthread.h> 
#include <stdlib.h>
#include <time.h>

#define N 100

int tab [N];//= {64, 34, 25, 12, 22, 11, 90};
int finish = 0;  // permet de finir mon thread, j'aurai pu utiliser pthread_cancel
pthread_mutex_t verrou; 

/* @param : 2 pointeurs vers des entiers
 * @brief : permet d'inverser les valeurs de 2 entiers
 */
void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

/*
 * @brief : fonction qui effectue le trie bubbleSort,
 *          utilise, les mutex pour les acces aux tableaux,
 *          et permettre d'avoir un affichage, a chaque 
 *          étape du trie
 */
void* bubbleSort()
{
	pthread_mutex_lock(&verrou); // mutex vaut 0

	int i, j;
	for (i = 0; i < N-1; ++i){
		for (j = 0; j < N-i-1; ++j) 
			if (tab[j] > tab[j+1]){
				swap(&tab[j], &tab[j+1]);
				pthread_mutex_unlock(&verrou); // incremente mutex, pour debloquer le 2nd thread
				sleep(1); // on est sur de debloquer le thread d'affichage
				pthread_mutex_lock(&verrou); // decremente mutex pour bloquer ce thread

			}
	}
	finish = 1; // on dit au 2nd thread de ce terminer
	pthread_mutex_unlock(&verrou); // on deverouille le 2nd thread pour finir
}
/*
 * @brief : fonction qui effectue l affichage du trie bubbleSort,
 *          utilise, les mutex pour les acces aux tableaux,
 *          et permettre d'avoir un affichage, a chaque 
 *          étape du trie
 */ 
void* printArray()
{
	pthread_mutex_lock(&verrou); // mutex < 0 donc attend
	
	do{
 		
		int i;
		for (i=0; i < N; i++)
			printf("%d ", tab[i]);
			printf("\n");
			pthread_mutex_unlock(&verrou); // incremente mutex, pour debloquer le 2nd thread
			sleep(1); // on est sur de debloquer le thread de triage
			pthread_mutex_lock(&verrou); // decremente mutex pour bloquer ce thread
	} while (!finish);

}
 
// Driver program to test above functions
int main()
{
 	int n = N;   
 	srand(time(NULL)); // initialisation de rand
	int i;
	for (i = 0; i < N; ++i){
		tab[i] = rand() % 100;			
	}	 // remplie le tableau de maniere aléatoire
	pthread_t threada;
	pthread_t threadb;
	pthread_mutex_init(&verrou, NULL); 
  if(pthread_create(&threada, NULL, bubbleSort, NULL) == -1) {
		perror("pthread_create");
		return -1;
  } // lance le thread bubble sort

	sleep(1);
  if(pthread_create(&threadb, NULL, printArray, NULL) == -1) {
		perror("pthread_create");
		return -1;
  } // lance le thread d affichage

	pthread_join(threada, NULL);
	pthread_join(threadb, NULL);

  return 0;
}

/* trace (7 elements) :
42 32 86 40 44 8 87 
42 32 40 86 44 8 87 
42 32 40 44 86 8 87 
42 32 40 44 8 86 87 
32 42 40 44 8 86 87 
32 40 42 44 8 86 87 
32 40 42 8 44 86 87 
32 40 8 42 44 86 87 
32 8 40 42 44 86 87 
8 32 40 42 44 86 87 
*/
