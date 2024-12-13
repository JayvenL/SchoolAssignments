#include "part2.hpp"

// Global semaphore array


const char *SEMAPHORE_NAMES[5] = {"/sem1", "/sem2", "/sem3", "/sem4", "/sem5"};

struct sharedData *shared_data; // Our shared data strucute which will be shared through processes
sem_t *semaphores[5];

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
        sem_unlink("/shared_sem"); // Destroy each semaphore
    }
    
}

// Function to get the next student number from the file
int get_next_student() {
    static int current_index = 0;

    int student_num = shared_data->students[current_index];
    current_index = (current_index + 1) % NUMSTUDENTS;

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

    //shared_data->current_index = 0;
    file.close();

}

bool sem_aquire_check(int ta){
    int j = ta -1;
    int jnext = ta % 5;

    sem_wait(semaphores[j]);

    int return_val = sem_trywait(semaphores[jnext]);
    if (return_val == 0){
        return true;
    }
    else{
        sem_post(semaphores[j]);
        //cout << "Semaphore of TA "<< ta << " +1 is locked, unlocking both semaphores." << endl;
        return false;
    }

}

void sem_release(int ta){
    int j = ta -1;
    int jnext = ta % 5;
    sem_post(semaphores[j]);
    sem_post(semaphores[jnext]);
}

// This function is the whole ta marking process
void ta_process(int ta_id) {
    string filename = "TA" + to_string(ta_id) + ".txt";
    ofstream ta_file(filename);
    

    srand(time(NULL) + ta_id);

    for (int i = 0; i<3; i++){ // itterate over the database 3 times

        while (true) {
            
            cout << "TA " << ta_id << " accessing database" << endl;

            if(!sem_aquire_check(ta_id)){
                sleep(1);
                continue;
            }

            // Access shared memory to get the next student
            int student = get_next_student();

            // Release the semaphores after accessing the shared data
            sem_release(ta_id);
            

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

    // if (ftruncate(shm_fd, sizeof(sharedData)) == -1) {
    //     perror("ftruncate");
    //     exit(1);
    // }
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