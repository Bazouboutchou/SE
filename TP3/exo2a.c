// http://www.lb.refer.org/chebaro/page5_4.htm
#include <stdio.h>
#include <fcntl.h>           /* Pour les constantes O_* */
#include <sys/stat.h>        /* Pour les constantes des modes */
#include <semaphore.h>
#include <string.h>

sem_t *fournisseur;

sem_t *fournisseurXT;
sem_t *fournisseurXA;

sem_t *fournisseurYA;
sem_t *fournisseurYP;


sem_t *fournisseurZP;
sem_t *fournisseurZT; 

void fournisseurs (int i){
					
	while(1){
				
		switch(i){
			case 0:						
				sem_wait(fournisseur);
				printf("Fournisseur X \n");
				sem_post(fournisseurXT);
				sem_post(fournisseurXA);
				sleep(1);
				break;
			case 1:
				sem_wait(fournisseur);
				printf("Fournisseur Y \n");
				sem_post(fournisseurYA);
				sem_post(fournisseurYP);
				sleep(1);
				break;
			case 2:
				sem_wait(fournisseur);
				printf("Fournisseur Z \n");
				sem_post(fournisseurZP);
				sem_post(fournisseurZT);
				sleep(1);
				break;
			default: 
				break;
		}
	}
	
}

void consommateurs (int i){
		
	switch(i){
		case 0:
			printf("Consommateur A \n");

			sem_wait(fournisseurXA);
			printf("A achete une allumette chez X\n");
					
			sem_wait(fournisseurXT);
			printf("A achete du tabac chez X\n");

			printf("A fume\n");
			sleep(1);
			printf("A a fini de fumer\n");

			sem_post(fournisseur);
			break;
		case 1:
			printf("Consommateur B \n");

			sem_wait(fournisseurYA);
			printf("B achete une allumette chez Y\n");

			sem_wait(fournisseurYP);
			printf("B achete du papier chez Y\n");
			
			printf("B fume\n");
			sleep(1);
			printf("B a fini de fumer\n");

			sem_post(fournisseur);
			break;
		case 2:
			printf("Consommateur C \n");

			sem_wait(fournisseurZP);
			printf("C achete du papier chez Z\n");

			sem_wait(fournisseurZT);
			printf("C achete du tabc chez Z\n");

			printf("C fume\n");
			sleep(1);
			printf("C a fini de fumer\n");

			sem_post(fournisseur);
			break;
		default: 
			break;
	}
	exit(0);
}

int main (){
	int i;
	sem_unlink ("fournisseur");
	sem_unlink ("fournisseurXA"); 	
	sem_unlink ("fournisseurXT");
	sem_unlink ("fournisseurYA");
	sem_unlink ("fournisseurYP"); 	
	sem_unlink ("fournisseurZP");
	sem_unlink ("fournisseurZT");

	fournisseur = sem_open ("fournisseur", O_CREAT|O_EXCL, 0644, 1);

	fournisseurXT = sem_open ("fournisseurXT", O_CREAT|O_EXCL, 0644, 0);
	fournisseurXA = sem_open ("fournisseurXA", O_CREAT|O_EXCL, 0644, 0);
	
	fournisseurYA = sem_open ("fournisseurYA", O_CREAT|O_EXCL, 0644, 0);
  fournisseurYP = sem_open ("fournisseurYP", O_CREAT|O_EXCL, 0644, 0);
	
	fournisseurZP = sem_open ("fournisseurZP", O_CREAT|O_EXCL, 0644, 0);
	fournisseurZT = sem_open ("fournisseurZT", O_CREAT|O_EXCL, 0644, 0);

	sleep(1);
		
	// creer autant de processus que de fournisseur 
	for (i = 0; i < 3; ++i){
		if (!fork()){
			fournisseurs(i);
		}
	}	
	// creer autant de processus que de consommateur 
	

	for (i = 0; i < 3; ++i){
		if (!fork()){
			consommateurs(i);
		}
	}	

	wait(NULL);
	sem_unlink ("fournisseur");
	sem_unlink ("fournisseurXA"); 	
	sem_unlink ("fournisseurXT");
	sem_unlink ("fournisseurYA");
	sem_unlink ("fournisseurYP"); 	
	sem_unlink ("fournisseurZP");
	sem_unlink ("fournisseurZT");

}
