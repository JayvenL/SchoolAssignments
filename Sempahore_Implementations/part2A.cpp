#include "part2.hpp"

// Global semaphore array
sem_t* semaphores[NUM_TAS];
vector<int> studentIds; // for later

// Function to initialize semaphores
void init_semaphores() {
    char sem_name[20];
    for (int i = 0; i < NUM_TAS; i++) {
        snprintf(sem_name, sizeof(sem_name), "/ta_sem_%d", i);
        semaphores[i] = sem_open(sem_name, O_CREAT, 0644, 1);
        if (semaphores[i] == SEM_FAILED) {
            perror("sem_open");
            exit(1); 
        }
    }
}

// Function to clean up semaphores
void cleanup_semaphores() {
    char sem_name[20];
    for (int i = 0; i < NUM_TAS; i++) {
        sem_close(semaphores[i]);
        snprintf(sem_name, sizeof(sem_name), "/ta_sem_%d", i);
        sem_unlink(sem_name);
    }
}

// Function to get the next student number from the file
int get_next_student(ifstream& file) {
    int student_num;
    if (!(file >> student_num)) {
        file.clear();
        file.seekg(0, ios::beg);
        file >> student_num;
    }
    return student_num;
}

// This function is the whole ta marking process
void ta_process(int ta_id) {
    string filename = "TA" + to_string(ta_id) + ".txt";
    ofstream ta_file(filename);
    ifstream student_list("database.txt");

    if (!ta_file.is_open() || !student_list.is_open()) {
        cerr << "File opening failed" << endl;
        exit(1);
    }

    srand(time(NULL) + ta_id);

    for (int i = 0; i<3; ++i){ // itterate over the database 3 times

        while (true) {
            // Lock semaphores of TAj and TA j+1
            sem_wait(semaphores[ta_id - 1]);
            sem_wait(semaphores[ta_id % NUM_TAS]);

            cout << "TA " << ta_id << " accessing database" << endl;

            // Access database
            int student = get_next_student(student_list);
            

            // Access Database random time with random number generated between 0 and 5
            sleep(rand() % 5);

            // Unlock semaphores post accessing database
            sem_post(semaphores[ta_id - 1]);
            sem_post(semaphores[ta_id % NUM_TAS]);

            cout << "TA " << ta_id << " marking student " << student << endl;

            // Marking process
            int mark = rand() % 11;
            ta_file << "Student " << student << ": " << mark << endl;
        

            // Simulate marking time with random number generated between 0 and 10
            sleep(rand() % 11);

            if (student == 9999) break;
        }
    }

    ta_file.close();
    student_list.close();
}

int main() {
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
    cleanup_semaphores();

    return 0;
}