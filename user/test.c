#include "kernel/types.h"
#include "user/user.h"

#define NULL 0

//Nicholas Heleta & Grace Biemans
/**
    This function performs the calculation we want the child processes to run
    @param K: the first argument given from the command line
    @param N: either 1st, 2nd, or 3rd argument from command line (main arguments)
*/
int summationFunction(int K, int N) {
    int sum1 = 0;
    int i = 1;
    while (i <= K) {
        int j = 1;
	    int sum2 = 0;
        while (j <= N) {
	        sum2 += (i - j);
            j++;
        }
	    sum1 += sum2;
        i++;
    }
    return sum1;
}

int main (int argc, char *argv[]) {

    // we need 4 parameters from command line
    if (argc != 5) {
        printf("Error: %d arguments given, require 4\n", argc);
        exit(1);
    }

    // Convert the strings to int
    int K = atoi(argv[1]);
    int L = atoi(argv[2]);
    int M = atoi(argv[3]);
    int N = atoi(argv[4]);



    // Error handling
    if (K < 0){
        printf("Error: Negative integer given.\n");
        exit(1);
    }
    if (L < 0){
        printf("Error: Negative integer given.\n");
        exit(1);
    }
    if (M < 0){
        printf("Error: Negative integer given.\n");
        exit(1);
    }
    if (N < 0){
        printf("Error: Negative integer given.\n");
        exit(1);

    }


    // group 1 - 3 turnaround times and estimated running time 
    uint turnaround1 = 0;
    uint turnaround2 = 0;
    uint turnaround3 = 0;
    uint estRuntime1 = 0;
    uint estRuntime2 = 0;
    uint estRuntime3 = 0;


    int group1[15];
    int group2[15];
    int group3[15];

    uint turn;
    uint runtime;

    volatile int var = 0;
    // forking
    for (int i = 0; i < 45; i++) {
        
    int pid1 = fork();

	if(i< 15){
		group1[i] = pid1;
		if (pid1 < 0) {
            		printf("failed to fork\n");
            		exit(1);
        	}
        	else if (pid1 == 0) {
            		var = summationFunction(K, N);
            		exit(0);
        	}
	}
        else if (i < 30){
		group2[i-15] = pid1;
        	if (pid1 < 0) {
            		printf("failed to fork\n");
            		exit(1);
        	}
        	else if (pid1 == 0) {
            		var = summationFunction(K, M);
            		exit(0);
        	}
	}
	else if (i < 45){
		group3[i-30] = pid1;
        	if (pid1 < 0) {
            		printf("failed to fork\n");
            		exit(1);
        	}
        	else if (pid1 == 0) {
            		var = summationFunction(K, L);
            		exit(0);
        	}
	}
    }

    // required check so the compiler actually does the sum function
    if(var != 0){
        printf("%d",var);
    }
    // waiting 
    for(int x=0; x < 45;x++){
        
        int pid = waitstat(NULL,&turn,&runtime);
    
        
        for(int y = 0; y < 15;y++){
            if(pid == group1[y]){
                turnaround1 = turnaround1 + turn;
                estRuntime1 = estRuntime1 + runtime;
                break;
            }
            else if (pid == group2[y]){
                turnaround2 = turnaround2 + turn;
                estRuntime2 = estRuntime2 + runtime;
                break;
            }
            else if (pid == group3[y]){
                turnaround3 = turnaround3 + turn;
                estRuntime3 = estRuntime3 + runtime;
                break;
            }
        }
    }

    printf("Turnaround time for group 1(N) is: %d, Estimated Runtime for group 1(N) is: %d.\n",turnaround1,estRuntime1);
    printf("Turnaround time for group 2(M) is: %d, Estimated Runtime for group 2(M) is: %d.\n",turnaround2,estRuntime2);
    printf("Turnaround time for group 3(L) is: %d, Estimated Runtime for group 3(L) is: %d.\n",turnaround3,estRuntime3);


    // Terminate program
    exit(0);
}
