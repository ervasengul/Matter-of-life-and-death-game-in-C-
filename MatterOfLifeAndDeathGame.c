#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void kill_processes(int n) {
    int remaining = n;
    int eliminated[n]; // To track if a person is eliminated
    int current_count = 0; // To keep track of the number of people still in the game

    // Initially, no one is eliminated (0 means alive)
    for (int i = 0; i < n; i++) {
        eliminated[i] = 0; // Mark everyone as alive initially
    }

    // Loop until only one person remains
    while (remaining > 1) {
        for (int i = 0; i < n; i++) {
            if (eliminated[i] == 0) { // If the person is still alive
                current_count++;

                // Every second person gets eliminated
                if (current_count == 2) {
                    // Create a child process to handle elimination
                    pid_t pid = fork();
                    
                    if (pid == 0) { // Child process
                        printf("Person %d eliminated\n", i + 1);
                        exit(0); // Child process exits after printing
                    } else if (pid > 0) { // Parent process
                        wait(NULL); // Parent waits for the child to finish
                        eliminated[i] = 1; // Mark this person as eliminated
                        current_count = 0; // Reset the count after each elimination
                        remaining--; // Decrease the number of remaining people
                    } else { 
                        perror("Fork failed");
                        exit(1);
                    }
                }
            }
        }
    }

    // After elimination, print the last remaining person
    for (int i = 0; i < n; i++) {
        if (eliminated[i] == 0) {
            printf("The last remaining person is %d\n", i + 1);
            break;
        }
    }
}

int main() {
    int n;

    // Input
    printf("Enter the number of people (n): ");
    scanf("%d", &n);

    if (n <= 0) {
        printf("Invalid input. The number of people must be greater than 0.\n");
        return 1;
    }

    // Call the function
    kill_processes(n);

    return 0;
}
