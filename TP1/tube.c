#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int main (){
	pid_t pid1, pid2;
	char str[100];
	int tot = 0;
	char c = 1;
	char tabCarac[255];
	int i;
	int test = 1;
	int fd1 [2];
	int fd2 [2];
	pipe (fd1);
	pipe (fd2);

	if ((pid1 = fork())){

		//if ((pid2 = fork())){
		
			do {

				if ((test = read(0, &c, 1)) == -1){
					exit(4);
				}
				if (isdigit(c)){
					write(fd1[1], &c, 1);
				} else{
					write(fd2[1], &c, 1);
				}
				
			}	while(test);			
			c = 0;						
			write (fd1[1], &c, 1);
			//write (fd2[1], &c, 1);
			read(fd2[0], &tot, sizeof(int));
			
			//read(desc1[0], &tabCarac, );
 			printf ("\nResultat de la somme : %d\n", tot);
			exit (0);	
			/*for(i = 0; i < strlen(tabCarac); ++i){
				printf("");	*/		
			/*}
			
		} else {  fils 2

		}*/
	} else { // fils 1
		close(fd1[1]);
		close(fd2[0]);
		do{
			read(fd1[0], &c, 1);

																															
			tot += atoi(&c);
		}while (c != 0);
		printf("tot %d", tot);
		write(fd2[1], &tot, sizeof(int));
		exit(0);
	}

	
}
