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
	int tabCarac[255];
	int i;
	int test = 1;
	int fd1 [2];
	int fd2 [2];
	pipe (fd1);
	pipe (fd2);

	if ((pid1 = fork())){

		if ((pid2 = fork())){
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
			sleep(1);
			write (fd2[1], &c, 1);			
									
			read(fd1[0], tabCarac, sizeof(int)*255);
 						

			for(i = 97; i < 123; ++i){
				printf("%c : %d\n", (char)i, tabCarac[i]);			
			}		

			read(fd2[0], &tot, sizeof(int));
			
			printf ("\nResultat de la somme : %d\n", tot);
			wait(NULL);
			exit (0);	
			
		} else {  
			for (i = 0; i < 255; ++i){
				tabCarac[i] = 0;
			}
			do{
				read(fd2[0], &c, 1);
																											
				++tabCarac[c];
			}while (c != 0);
			sleep(1);
			write(fd1[1], tabCarac, sizeof(int)*255);
			exit(0);

		}
	} else { // fils 1

		do{
			read(fd1[0], &c, 1);
																															
			tot += atoi(&c);
		
		}while (c != 0);		
		sleep(1);
					
		write(fd2[1], &tot, sizeof(int));
		exit(0);
	}

	
}
