#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void deroute (int numeroSignal){
	printf("Le fils sarrete avec le signal %d", numeroSignal);
}

int main (){
	pid_t pid;
	signal (SIGINT, deroute);

	if ((pid = fork())){
		sleep(1);
		kill(pid, SIGINT);
		wait(NULL);
	} else {
		pause();
	
	}
	
}
