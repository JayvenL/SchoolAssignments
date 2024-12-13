/*
* SYSC4001 A2 Part2 
*
* Authors: Aj Donald 101259149 & Jayven Larsen 101260364
*
*/

#include <iostream>
using namespace std;
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>


// semaphore structures for operations (P and V, same as parent process)
struct sembuf sem_lock = {0, -1, 0}; // P: wait/decrement
struct sembuf sem_unlock = {0, 1, 0}; // V: signal/increment

int main() {
    srand(time(0));
    
    // similarly to the other file, create a key before shared memory allocation occurs (shmget)
    key_t key = ftok("shmfile", 65); // same key as Process 1 
    int shmid = shmget(key, sizeof(int), 0666); 
    
    // error checking
    if (shmid < 0) {
        cerr << "Process 2: Failed to access shared memory" << endl;
        exit(1);
    }

    // attach the segment
    int *shared_var = (int *)shmat(shmid, nullptr, 0);

    // more error checking
    if (shared_var == (void *)-1) {
        cerr << "Process 2: Failed to attach shared memory" << endl;
        exit(1);
    }

    // access sempahore
    int semid = semget(key, 1, 0666);
    if (semid < 0) {
        cerr << "Process 2: Failed to access semaphore" << endl;
        exit(1);
    }

    while (*shared_var != 0) {
        
        // lock the semaphore before using shared memory (similarly to parent process)
        *shared_var = rand() % 11;
        semop(semid, &sem_lock, 1); 
        cout << "Process 2: Shared random number is: " << *shared_var << endl;
        sleep(1);
        

        // unlock the semaphore after reading shared memory
        semop(semid, &sem_unlock, 1); 
    }

    // detach shared memory
    shmdt(shared_var);
    cout << "Process 2 finished" << endl;

    return 0;
}
