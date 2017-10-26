
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdio.h>
void exo1(){
	while (1){
		printf("Le PID est : %d", getpid());
		printf("\nLe PPID est : %d \n", getppid());	
	} 

}

void exo2(){
	int pid;
	int err = 0;
	FILE * fd;
	fd = fopen ("listeNbPremier.txt", "r");
	char buf [50];
	
	if ((pid = fork())){
		fread (buf, 2, 10, fd); // un char fait 1 octet mais faut prendre en compte l'espace
		printf ("Pere : %s \n", buf);
	} else {
		sleep (2);
		fseek(fd, 20, SEEK_SET);
		fread (buf, 3, 20, fd); // un char fait 1 octet mais faut prendre en compte l'espace
		printf ("Fils : %s \n", buf);
	}
	
	
	wait(&err);
	if (err == -1) printf("Error sur le fils");
	if (pid){
		while (fread (buf, 2, 50, fd)); // jusqu'a la fin du fichier
		printf ("Pere : %s \n", buf);
	}
	fclose (fd);
	
}


int main (){
	//exo1();
	exo2();
}
