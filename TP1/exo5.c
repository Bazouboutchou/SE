//http://supertos.free.fr/supertos.php?m=1&page=762

#include <stdio.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */


/* 
 * @param : taille du segement, nom du fichier, entier qui permet de genere la cle
 * @return : retourne l identifiant du segment de memoire crée 
 * @brief : fonction qui creer un segment de memoire. Il affichera la 
 *					cle generer ainsi que l identifiant du segment
 */
int creeSegment (int taille, char *ficNom, int fic){
	key_t key = ftok(ficNom, fic); // genere la cle	
	int shmid;
	if ((shmid = shmget(key, taille, IPC_CREAT|IPC_EXCL)) == -1){
		perror("shmget");
		return -1;	
	} // creer le segment de donnee et recupere l'identifiant
	printf ("Cle : %d \n", key);
	printf ("Identifieur : %d", shmid);	
	return shmid;
}

int main (){
	char *nom = "partage.txt";	
	creeSegment (10, nom, 1);
	return 1;
}

/*
trace de l'execution :
a)
Le PID est : 3314
Le PPID est : 1427 
b)
Pere : 2 3 5 7 11 13 17 19 23 29 
Fils : 31 37 41 43 47 53 59 61 67 71 73 79 83 89 97 101 103 107 109 113 
Pere : 31 37 41 43 47 53 59 61 67 71 73 79 83 89 97 101 103 107 109 113 127 131 137 139 149 151 157 163 167 173 179 181 191 
*/
