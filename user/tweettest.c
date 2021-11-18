#include "kernel/types.h"
#include "user/user.h"

// Userspace program to test tweets
int main(){

    printf("Starting test suite...\n");

    int failed = 0;
    char teststring[141];
    char teststring2[141];
    char teststring3[141];
    char teststring4[141];

    int test = tput(1,"test message");
    int invalid_tag = tput(9, "example message");
    int test2 = tget(1, teststring);
    int test3 = tget(1, teststring2);

    if(invalid_tag != -1){
        printf("Test failed// Return value for putting invalid tag %d\n", invalid_tag);
        failed++;
    }
    if(test != 0){
        printf("Test failed// Return value for putting: %d\n",test);
        failed++;
    }
    if(test2 != 0){
        printf("Test failed// Return value for getting: %d\nString given back %s\n",test2, teststring);
        failed++;
    }
    if(test3 != -1){
        printf("Test failed// Return value for getting with no tweets availible: %d\n",test3);
        failed++;
    }
    

    tget(1,teststring4);

    int holder;
    int rc = fork();

    if(rc < 0){
        printf("failed fork");
        exit(1);
    }
    else if (rc == 0){
        int gettest = btget(2,teststring3);
        if(gettest != 0){
            printf("Test failed// btget did not return 0\n");
            failed++; 
        }
        exit(gettest);
    }
    else{
        tput(2,"btget test message");
        wait(&holder);
    }

    tput(1,"1");
    tput(1,"2");
    tput(1,"3");
    tput(1,"4");
    tput(1,"5");
    tput(1,"6");
    tput(1,"7");
    tput(1,"8");
    tput(1,"9");
    tput(1,"10");
    

    int rc2 = fork();

    if(rc2 < 0){
        printf("failed fork");
        exit(1);
    }else if (rc2 == 0){
        int gettest = btput(1,teststring3);
        if(gettest != 0){
            printf("Test failed// btput did not return 0\n");
            failed++; 
        }
        exit(gettest);
    }
    else{
        tget(1,teststring3);
        wait(0);
    }

    // empty out the struc
    char empty[141];
    tget(1,empty);
    tget(1,empty);
    tget(1,empty);
    tget(1,empty);
    tget(1,empty);
    tget(1,empty);
    tget(1,empty);
    tget(1,empty);
    tget(1,empty);
    tget(1,empty);

    // batch put's
    for(int x = 0; x < 5; x++){
        int rc3 = fork();
        if(rc3 < 0){
            exit(1);
        }
        else if (rc3 == 0){
            int test_val = tput(3,"message");
            if(test_val != 0){
                printf("failed to put");
                exit(1);
            }
            exit(0);
        }
    }

    for(int y = 0; y <5; y++){
        wait(0);
    }

    // batch get's
    for(int x = 0; x < 5; x++){
        int rc3 = fork();
        if(rc3 < 0){
            exit(1);
        }
        else if (rc3 == 0){
            char empty_space[141];
            int test_val = tput(3,empty_space);
            if(test_val != 0){
                printf("failed to get");
                exit(1);
            }
            exit(0);
        }
    }

    for(int y = 0; y <5; y++){
        wait(0);
    }


    printf("Test suite finished with %d failures in the parent space.\n",failed);
    // Exit properly
    exit(0);
}
