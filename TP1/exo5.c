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


