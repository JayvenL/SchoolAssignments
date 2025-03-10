#include "part2.hpp"

// Global semaphore array
sem_t *semaphores[5];

// These are used to open the semaphores with according names. (Was not working with sem_init)
const char *SEMAPHORE_NAMES[5] = {"/sem1", "/sem2", "/sem3", "/sem4", "/sem5"};

struct sharedData *shared_data; // Our shared data strucute which will be shared through processes


// Function to initialize semaphores
void init_semaphores() {
    for (int i = 0; i < SEMNUM; i++) {
        semaphores[i]=sem_open(SEMAPHORE_NAMES[i], O_CREAT | O_EXCL, 0644, 1); 
           }
}


// Function to clean up semaphores
void cleanup_semaphore() {
    for (int i = 0; i < 5; i++) {
        sem_close(semaphores[i]);
    }
    
}

// Function to get the next student number from the file
int get_next_student() {
    static int index = 0;

    int student_num = shared_data->students[index];
    index = (index + 1) % NUMSTUDENTS;

    return student_num;

    
}


// We will now be loading the student id's into an integer array wihtin a
//pointer to *shared_data type sharedData struct

void load_students(){ 
    ifstream file("database.txt");
    if (!file.is_open()) {
        std::cerr << "Failed to open student_list.txt" << std::endl;
        exit(1);
    }

    for (int i =0; i < NUMSTUDENTS; i++){
        file >> shared_data->students[i];
    }

    file.close();

}

void acquire_semaphores_ordered(int first, int second) {
    if (first < second) {
        sem_wait(semaphores[first]);
        sem_wait(semaphores[second]);
    } else {
        sem_wait(semaphores[second]);
        sem_wait(semaphores[first]);
    }
}

void sem_release(int first, int second) {
    sem_post(semaphores[first]);
    sem_post(semaphores[second]);
}

// This function is the whole ta marking process
void ta_process(int ta_id) {
    string filename = "TA" + to_string(ta_id) + ".txt";
    ofstream ta_file(filename);
    

    srand(time(NULL) + ta_id);

    for (int i = 0; i<3; i++){ // itterate over the database 3 times

        while (true) {
            int first_semaphore = ta_id - 1;       
            int second_semaphore = ta_id % 5;
            
            cout << "TA " << ta_id << " accessing database" << endl;

            acquire_semaphores_ordered(first_semaphore, second_semaphore);

            // Access shared memory to get the next student
            int student = get_next_student();

            // Release the semaphores after accessing the shared data
            sem_release(first_semaphore, second_semaphore);
            

            // Access Database random time
            sleep(rand() % 5);
            
            cout << "TA " << ta_id << " marking student " << student << endl;

            // Marking process
            int mark = rand() % 11;
            ta_file << "Student " << student << ": " << mark << endl;
            

            // Simulate marking time
            sleep(rand() %11);

            if (student == 9999) {
                cout << "TA " << ta_id << " reached end of list." << endl;
                break;
            }
        }
    }

    ta_file.close();
    
}

int main() {

     // Create shared memory
    int shm_fd = shm_open("/ta_shared_mem", O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(1);
    }
    //Truncate fucntions used for the following shared data mapping
    ftruncate(shm_fd, sizeof(sharedData));

    shared_data =static_cast<sharedData*>(mmap(NULL, sizeof(sharedData), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0));
    if (shared_data == MAP_FAILED) {
        perror("mmap");
        
    }

    load_students();
    init_semaphores();

    // Create TA processes
    pid_t pids[NUM_TAS];
    
    for (int i = 0; i < NUM_TAS; i++) {
        pids[i] = fork();
        
        if (pids[i] == 0) { // Child process
            ta_process(i+1);
            exit(0);
        } else if (pids[i] < 0) { // Fork failed
            perror("fork");
            exit(1);
        }
    }

    // Wait for all TA processes to finish
    for (int i = 0; i < NUM_TAS; i++) {
        waitpid(pids[i], NULL, 0);
    }

    // Cleanup semaphores
    
    cleanup_semaphore();
    munmap(shared_data, sizeof(sharedData));
    shm_unlink("/ta_shared_mem");
    

    return 0;
}