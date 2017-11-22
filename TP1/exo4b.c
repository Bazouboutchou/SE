#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int MaxVal = 100;
FILE *fdFile; 

/* @param : descripteur de fichier avec le pere 
 * @brief : la fonction commence par recuperer le premier nombre recu 
 * dans le descripteur de fichier (cette eniter est 1er), puis on va parcourir 
 * tout les entiers envoyés par le pere, on regarde si le nombre est divisible
 * par le 1er nombre recuperer. Si il ne l'ai pas en l'envoi au prochain fils
 */
void vie_des_fils(int *fd){
	int entierPremier;
	int entier;
	close(fd[1]); // fermer en ecriture 

	read (fd[0], &entierPremier, sizeof(int)); // le premier entier retourné est 1er		
	if (entierPremier != -1){ // si on a tjs des elements
		printf(" %d \n", entierPremier); 
		fprintf(fdFile, "%d ", entierPremier); 
		fflush(fdFile); // vide buffer sinon probleme
		int fd2[2]; // pour communiquer avec le fils;

		pipe (fd2);

		if (fork()){ // pere 
			close(fd2[0]); // fermer en lecture 

			do{
				read(fd[0], &entier, sizeof(int));
		
				if (entier % entierPremier != 0 && entier != -1){
					write (fd2[1], &entier, sizeof(int));
				}	// on lit les entiers 1 par 1 et si ils ne sont pas divisible par 
					// entierPremier, on les envoies au fils
			} while (entier != -1);
			entier = -1;
			write(fd2[1], &entier, sizeof(int));
			wait(NULL);
		} else{ // fils
			vie_des_fils(fd2);
			return;	
		}	
	}

}

int main(){
	int fd[2];
	pid_t pid; 
	pipe(fd);
	fdFile = fopen("cribleErastosthene.txt", "w");	// fichier contenant les nombres 1er
	if(pid = fork()){ // pere
		close (fd[0]); // fermer en lecture		
		int i;
		for (i = 2; i <= MaxVal; ++i){
			write(fd[1], &i, sizeof(int)); // on ecrit les entiers 1 par 1 
		}
		i = -1;
		write(fd[1], &i, sizeof(int)); // on envoi -1 pour que fils connaisse la fin des entiers
		wait(NULL);
	}	else{ // fils F2
		vie_des_fils(fd);
		exit(0);	
	}

	fclose(fdFile);
}


/* trace pour MaxVal = 100 :
 2 
 3 
 5 
 7 
 11 
 13 
 17 
 19 
 23 
 29 
 31 
 37 
 41 
 43 
 47 
 53 
 59 
 61 
 67 
 71 
 73 
 79 
 83 
 89 
 97 
*/
