#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

int main (){
	pid_t pid;
	if ((pid = fork())){ // pere
		sleep(1);
		kill(pid, SIGINT); // tue le fils
		wait(NULL);
		printf("Le fils est arrete");
	} else { // fils
		while (1){
			printf("Le fils continue \n");		
		}
	}
	
}


/* trace : 
.........
Le fils continue 
Le fils continue 
Le fils continue 
Le fils continue 
Le fils continue 
Le fils continue 
Le fils continue 
Le fils continue 
Le fils continue 
Le fils continue 
Le fils continue 
Le fils est arrete
*/
