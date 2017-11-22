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
	int tot = 0;
	char c = 1;
	int tabCarac[255]; // permet la conversion des int vers les char avec un cast, l'indice carrespond au caractere ASCII.
	int i;
	int test = 1;
	int fd1 [2]; 
	int fd2 [2];
	pipe (fd1); 
	pipe (fd2);

	if ((pid1 = fork())){ // pere

		if ((pid2 = fork())){ // pere
			do {

				if ((test = read(0, &c, 1)) == -1){ // on lit sur l'entre standard
					exit(1);
				}
				if (isdigit(c) && c != 0){ // si le caractere lu est un nombre on ecrit dans le fils 2 
					write(fd1[1], &c, 1);
				} else{ // si le caractere lu est un char on ecrit dans le fils 2
					write(fd2[1], &c, 1);
				}
				
			}	while(test); // tant que lon a pas EOF
			c = 0;	// valeur pour stopper les boucles
			write (fd1[1], &c, sizeof(char)); // on stop le fils 1
			sleep(1); // on evite les read bloquant
			write (fd2[1], &c, sizeof(char));	// on stop le fils 2
									
			read(fd1[0], tabCarac, sizeof(int)*255); // on recupere le tableau avec le compte des caracteres
 						

// on va de 97 à 123 pour afficher seulement les caracteres alphabetiques en minuscule
			for(i = 97; i < 123; ++i){
			
				printf("%c : %d\n", (char)i, tabCarac[i]);			
			}	// on affiche les occurences des caracteres.

			read(fd2[0], &tot, sizeof(int)); // on lit la somme des int 
			
			printf ("\nResultat de la somme : %d\n", tot); // affiche la somme
			wait(NULL); // attend les fils
			
		} else {  // fils 2
			for (i = 0; i < 255; ++i){
				tabCarac[i] = 0;
			} // initilise le nombre des occurences à 0
			do{
				read(fd2[0], &c, 1); // lit ce que le pere envoi
																											
				++tabCarac[c]; // incremente le caractere corrspondant
			}while (c != 0); // tant que l'on recoit pas 0
			sleep(1);
			write(fd1[1], tabCarac, sizeof(int)*255); // renvoi au pere le tableau
			exit(0);

		}
	} else { // fils 1

		do{
			read(fd1[0], &c, sizeof(char)); // lit les entiers 
																															
			tot += atoi(&c); // conversion avec atoi en int et adition 
		
		}while (c != 0);		
		sleep(1);
					
		write(fd2[1], &tot, sizeof(int)); // renvoi le total au pere
		exit(0);
	}	
}

/* trace : 
azer123
a : 1
b : 0
c : 0
d : 0
e : 1
f : 0
g : 0
h : 0
i : 0
j : 0
k : 0
l : 0
m : 0
n : 0
o : 0
p : 0
q : 0
r : 1
s : 0
t : 0
u : 0
v : 0
w : 0
x : 0
y : 0
z : 1

Resultat de la somme : 6
*/
