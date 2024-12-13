#ifndef PART2_HPP
#define PART2_HPP

#define NUM_TAS 5
#define SEMNUM 5
#define NUMSTUDENTS 20



#include <iostream>
#include <semaphore.h>
#include <fstream>
#include <string>
#include <vector>
#include <sys/wait.h>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
using namespace std;




struct sharedData{

    int students[NUMSTUDENTS];
};

void load_students();
void acquire_semaphores_ordered(int first, int second);
bool sem_aquire_check(int ta);
void sem_release(int ta);
void init_semaphores();
void cleanup_semaphore();
int get_next_student();
void ta_process(int ta_id);

#endif