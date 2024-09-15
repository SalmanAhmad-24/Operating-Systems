#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int numProcesses;
    while (1) {
        
        
        // Ask the user how many processes to create
        printf("Enter the number of processes to create (0 to quit): ");
        scanf("%d", &numProcesses);

        if (numProcesses == 0) {
            // Quit the program on user input 0
            break;
        } else if (numProcesses < 0 || numProcesses > 9) {
            // Validate input range
            printf("Please enter a number between 0 and 9.\n");
            continue;
        }
        else{
          for(int i=0;i<numProcesses;i++){
            pid_t rc =fork();
            if(rc<0){
            printf("error \n");

            }
            else if(rc==0){
                printf("Hello world\n");
                exit(EXIT_SUCCESS);
            }
          }
            for(int  i=0;i<numProcesses;i++){
                wait(NULL);
            }
        
        }

    }

    return 0;
}