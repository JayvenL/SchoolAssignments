/*
* SYSC4001 A2 Part2 
*
* Authors: Aj Donald 101259149 & Jayven Larsen 101260364
*
*/

#include <iostream>
using namespace std;
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <time.h>

// semaphore structs for operations (P and V)
struct sembuf sem_lock = {0, -1, 0}; // P: wait/decrement
struct sembuf sem_unlock = {0, 1, 0}; // V: signal/increment

int main() {
    srand(time(0));

    // generate a key to use as an "id" for the shared memory
    key_t key = ftok("shmfile", 65); 
    
    // here we create shared memory segment (store it in shmid)
    // 0666 is a permission mode to use, otherwise IPC_CREAT means create if it doenst exist.
    int shmid = shmget(key, sizeof(int), 0666 | IPC_CREAT); // shmget, allocate the shared memory, 
    if (shmid < 0) {
        cerr << "Failed to create shared memory" << endl;
        exit(1);
    }

    // link the shared memory segment
    int *shared_var = (int *)shmat(shmid, nullptr, 0);
    if (shared_var == (void *)-1) {
        cerr << "Failed to attach shared memory" << endl;
        exit(1);
    }

    *shared_var = -1;

    // create the semaphore used.
    int semid = semget(key, 1, 0666 | IPC_CREAT);
    if (semid < 0) {
        cerr << "Failed to create semaphore" << endl;
        exit(1);
    }

    // initialize the semaphore to unlocked state (1)
    if (semctl(semid, 0, SETVAL, 1) < 0) {
        cerr << "Failed to initialize semaphore" << endl;
        exit(1);
    }

    pid_t pid = fork();

    if (pid < 0) {
        cerr << "Fork failed!" << endl;
        exit(1);
    } else if (pid == 0) {
        while (*shared_var != 0) {

            // lock the semaphore before using shared memory
            semop(semid, &sem_lock, 1); 

             // generate random number between 0 and 10
            *shared_var = rand() % 11;
            cout << "Process 1: Generated random number: " << *shared_var << endl;

            if (*shared_var > 5) {
                cout << "Process 1: High value" << endl;
            } else {
                cout << "Process 1: Low value" << endl;
            }
        
            // unlock the semaphore after done using accessing memory
            semop(semid, &sem_unlock, 1);
            
            // execute process 2 if the number is 9
            if (*shared_var == 9) {
                execl("/Users/jayvenlarsen/gitRepo/sysc4001/A2/Part2/Part_II_AjDonald_JayvenLarsen_program2", "Part_II_AjDonald_JayvenLarsen_program2", nullptr);
                cerr << "Failed to launch Process 2!" << endl;
                exit(1);
            }

            sleep(1); 
        }

        // detach memory
        shmdt(shared_var);
        cout << "Process 1 finished" << endl;
    } else {
        // parent waits for child to finish 
        waitpid(pid, nullptr, 0);

        // cleanup shared memory 
        shmctl(shmid, IPC_RMID, nullptr);
        cout << "Parent cleaned up shared memory" << endl;
        // cleanup the semaphore
        semctl(semid, 0, IPC_RMID);
    }

    return 0;
}
