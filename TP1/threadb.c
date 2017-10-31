//http://gkemayo.developpez.com/tempsreel/programmation-multitache-posix/
#include <stdio.h>
#include <pthread.h> 
#define N 7

int tab [N] = {64, 34, 25, 12, 22, 11, 90};
int finish = 0; 
pthread_mutex_t verrou; 
void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

 
// A function to implement bubble sort
void* bubbleSort()
{
	int i, j;
	for (i = 0; i < N-1; ++i){
		for (j = 0; j < N-i-1; ++j) 
			if (tab[j] > tab[j+1]){
				pthread_mutex_lock(&verrou); 
	
				swap(&tab[j], &tab[j+1]);
			}
		sleep(1);
	}
	finish = 1;
}
 
/* Function to print an array */
void* printArray()
{
//	sleep(1);
	do{
 		
		int i;
		for (i=0; i < N; i++)
		    printf("%d ", tab[i]);
		printf("\n");
 	    pthread_mutex_unlock(&verrou); 
 		sleep(1);
		
	} while (!finish);

}
 
// Driver program to test above functions
int main()
{
 	int n = N;   
	pthread_t threada;
	pthread_t threadb;
	pthread_mutex_init(&verrou, NULL); 

    if(pthread_create(&threada, NULL, bubbleSort, NULL) == -1) {
		perror("pthread_create");
		return -1;
    }

    if(pthread_create(&threadb, NULL, printArray, NULL) == -1) {
		perror("pthread_create");
		return -1;
    }
	pthread_join(threada, NULL);
	pthread_join(threadb, NULL);

    //printArray(tab, N);
    return 0;
}
