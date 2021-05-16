#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    int i;
    i = fork();
    if (i = 0){
        execvp(argv1,argv+1)
    }
    else{
        wait(NULL);
    }


}