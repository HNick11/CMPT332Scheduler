#include "kernel/types.h"
#include "user/user.h"


int main(int argc, char* argv[]){

// check for args

if(argc != 2){
	printf("Invalid argument\n");
	exit(1);
}

// begin forking

int n_loops = atoi(argv[1]);

int children[n_loops];

for(int x = 0; x < n_loops;x++){
	
	int f = fork();
	if (f < 0){
		printf("Failed to fork\n");
		exit(1);
	}
	else if (f == 0){
		// child process
		children[x] = f;
		exit(0);
	}
}


int n_procs = howmanycmpt();
printf("The number of proccesses with cmpt in the name is: %d\n", n_procs);

for(int y = 0 ; y < n_loops; y++){
	// parent waiting
	wait(&children[y]);
}


exit(0);
}
