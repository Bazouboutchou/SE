
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdio.h>

/*
 * @brief : fonction qui affiche le pid du processus ainsi que celui du pere
 *
 */
void exo1(){
	sleep(1);
	printf("Le PID est : %d", getpid());
	printf("\nLe PPID est : %d \n", getppid());	
}


/*
 * @brief : fonction, dans laquelle le pere les 10 premiers nombres
 *  			  ecrit dans un fichier, puis creer un fils qui va lire les 
 * 				  20 suivant, puis le pere lit jusqua la fin
 *
 */
void exo2(){
	pid_t pid;
	int err = 0;
	int i;
	int tmp;
	FILE * fd;
	fd = fopen ("listeNbPremier.txt", "r");
	
	printf ("Pere : ");

	for (i = 0; i < 10; ++i){
		fscanf (fd, "%d", &tmp);
		printf("%d ", tmp);
	} // affiche les 10 premiers
	printf("\n");
	if (!(pid = fork())){ // pere
	
		//sleep (5);
		printf ("Fils : ");

		for (i = 0; i < 20; ++i){
			fscanf (fd, "%d", &tmp);
			printf("%d ", tmp);
		} // affiche les 20 suivant
		printf("\n");
		fclose (fd);
		return;
	} else if (pid == -1){
		perror("fork");
		return;	
	}
	sleep(2);
	wait(&err); // on attend le fils
	if (err == -1) printf("Erreur sur le fils");
	printf ("Pere : ");
	while ((fscanf (fd, "%d", &tmp)) > 0){
		
		printf("%d ", tmp);
	}	// lit jusqua la fin du fichier
	printf("\n");
	fclose (fd);
}


int main (){
	exo1();
	exo2();
	return 1;
}

/* trace 
Le PID est : 4620
Le PPID est : 1427 
Pere : 2 3 5 7 11 13 17 19 23 29 
Fils : 31 37 41 43 47 53 59 61 67 71 73 79 83 89 97 101 103 107 109 113 
Pere : 31 37 41 43 47 53 59 61 67 71 73 79 83 89 97 101 103 107 109 113 127 131 137 139 149 151 157 163 167 173 179 181 191 
*/
