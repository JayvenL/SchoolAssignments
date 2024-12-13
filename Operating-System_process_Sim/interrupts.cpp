/** 
 * SYSC4001 Assignment 3 - Part 1 
 * 
 * Authors: Aj Donald 101259149, Jayven Larsen 101260364
 */


#include "interrupts_101259149_101260364.hpp" 


// initialize the a vector with the memory partitions, -1 since they're unoccupied by default. 
std::vector<MemoryPartition> memoryPartitions = {{40, -1}, {25, -1}, {15, -1}, {10, -1}, {8, -1}, {2, -1}};
std::queue<Process> readyQueue; 
uint16_t currentTime = 0; 


void logMemoryStatus(std::ofstream &memoryLog, const std::vector<MemoryPartition> &partitions) {
    static bool headerWritten = false;
    if (!headerWritten) {
        memoryLog << "+------------------------------------------------------------------------------------------+\n";
        memoryLog << "| Time of Event | Memory Used |      Partitions State     | Total Free Memory | Usable Free Memory |\n";
        memoryLog << "+------------------------------------------------------------------------------------------+\n";
        headerWritten = true;
    }

    unsigned int memoryUsed = 0;
    unsigned int totalFree = 0;
    unsigned int usableFree = 0;
    std::ostringstream partitionState;

    for (const auto &partition : partitions) {
        if (partition.occupiedBy != -1) {
            memoryUsed += partition.size;
        } else {
            totalFree += partition.size;
            usableFree += partition.size;
        }
        partitionState << (partition.occupiedBy == -1 ? "-1" : std::to_string(partition.occupiedBy)) << ", ";
    }

    // format the table 
    memoryLog << "| " << std::setw(12) << currentTime << " | " << std::setw(11) << memoryUsed << " | "
              << std::setw(25) << partitionState.str() << " | " << std::setw(17) << totalFree << " | "
              << std::setw(17) << usableFree << " |\n";
}


// function to log the execution of the program
void logExecutionStatus(std::ofstream &executionLog, int pid, const std::string &oldState, const std::string &newState) {
    static bool headerWritten = false;
    if (!headerWritten) {
        executionLog << "+------------------------------------------------+\n";
        executionLog << "| Time of Transition | PID | Old State | New State |\n";
        executionLog << "+------------------------------------------------+\n";
        headerWritten = true;
    }

    // format the table 
    executionLog << "| " << std::setw(18) << currentTime << " | " << std::setw(3) << pid << " | "
                 << std::setw(10) << oldState << " | " << std::setw(9) << newState << " |\n";
}

int allocateMemory(Process &process) {
    int bestFitIndex = -1;
    unsigned int smallestSize = 1000000;

    for (int i = 0; i < memoryPartitions.size(); i++) {
        if (memoryPartitions[i].occupiedBy == -1 && memoryPartitions[i].size >= process.memorySize) {
            if (memoryPartitions[i].size < smallestSize) {
                smallestSize = memoryPartitions[i].size;
                bestFitIndex = i;
            }
        }
    }

    if (bestFitIndex != -1) {
        memoryPartitions[bestFitIndex].occupiedBy = process.pid;
        process.memoryPartition = bestFitIndex;
    }

    return bestFitIndex;
}


// Function to deallocate memory
void deallocateMemory(Process &process) {
    if (process.memoryPartition != -1) {
        memoryPartitions[process.memoryPartition].occupiedBy = -1;
        process.memoryPartition = -1;
    }
}
// FCFS scheduler 
void runScheduler(std::queue<Process> &readyQueue, std::ofstream &executionLog, std::ofstream &memoryLog) {
    std::vector<std::pair<Process, int>> waitingProcesses; // a pair created for process + end IO time. 
    std::queue<Process> executionQueue;

    while (!readyQueue.empty() || !waitingProcesses.empty() || !executionQueue.empty()) {
        // allocate memory for newly arrived processes
        while (!readyQueue.empty() && readyQueue.front().arrivalTime <= currentTime) {
            Process process = readyQueue.front();
            readyQueue.pop();

            int partitionIndex = allocateMemory(process);
            if (partitionIndex != -1) {
                logMemoryStatus(memoryLog, memoryPartitions); // Log memory allocation
                logExecutionStatus(executionLog, process.pid, "NEW", "READY"); // Log state transition
                executionQueue.push(process); // Add process to execution queue
            } else {
                // requeue for memory allocation later
                readyQueue.push(process);
                break; // exit loop if no memory is available
            }
        }

        if (!executionQueue.empty()) {
            Process process = executionQueue.front();
            executionQueue.pop();

            // log the state transition 
            if (process.state != 1 /* RUNNING */) {
                logExecutionStatus(executionLog, process.pid, "READY", "RUNNING");
                process.state = 1; // state is now RUNNING 
            }

            process.remainingCPUTime--;
            currentTime++;

            // hadndle IO 
            if (process.ioFrequency > 0 &&
                (process.totalCPUTime - process.remainingCPUTime) % process.ioFrequency == 0 &&
                process.remainingCPUTime > 0) {
                logExecutionStatus(executionLog, process.pid, "RUNNING", "WAITING");

                // change the process state to waiting 
                int ioEndTime = static_cast<int>(currentTime) + process.ioDuration;
                waitingProcesses.push_back({process, ioEndTime});
            } else if (process.remainingCPUTime > 0) {
                executionQueue.push(process); // add back to the exec queue if not completed 
            } else {
                logExecutionStatus(executionLog, process.pid, "RUNNING", "TERMINATED");
                deallocateMemory(process);
                logMemoryStatus(memoryLog, memoryPartitions); 
            }
        } else if (!waitingProcesses.empty()) {
            // no process is running advance time to the next I/O completion
            int nextIOCompletionTime = waitingProcesses.front().second;
            for (const auto &pair : waitingProcesses) {
                nextIOCompletionTime = std::min(nextIOCompletionTime, pair.second);
            }
            currentTime = static_cast<uint16_t>(std::max(static_cast<int>(currentTime), nextIOCompletionTime));
        } else if (!readyQueue.empty()) {
            // If no processes are ready or waiting, skip to the next process arrival
            currentTime = static_cast<uint16_t>(std::max(static_cast<int>(currentTime), static_cast<int>(readyQueue.front().arrivalTime)));
        }

        for (auto it = waitingProcesses.begin(); it != waitingProcesses.end();) {
            auto &waitingProcess = it->first;
            int ioEndTime = it->second;

            if (currentTime >= ioEndTime) {
                // I/O completed transition back to READY state
                if (waitingProcess.state != 3 /* READY */) {
                    logExecutionStatus(executionLog, waitingProcess.pid, "WAITING", "READY");
                    waitingProcess.state = 3; // update the state to "READY"
                }
                executionQueue.push(waitingProcess);
                it = waitingProcesses.erase(it);
            } else {
                ++it;
            }
        }
    }
}

// priority scheduler (w/o preemption)
void runPriorityScheduler(std::queue<Process> &readyQueue, std::ofstream &executionLog, std::ofstream &memoryLog) {
    std::vector<std::pair<Process, int>> waitingProcesses; // 
    auto compare = [](const Process &a, const Process &b) {
        return (a.priority > b.priority) || (a.priority == b.priority && a.arrivalTime > b.arrivalTime);
    };
    std::priority_queue<Process, std::vector<Process>, decltype(compare)> executionQueue(compare); // priority queue 

    while (!readyQueue.empty() || !waitingProcesses.empty() || !executionQueue.empty()) {
        // allocate memory for recently arrived / new processes 
        while (!readyQueue.empty() && readyQueue.front().arrivalTime <= currentTime) {
            Process process = readyQueue.front();
            readyQueue.pop();

            int partitionIndex = allocateMemory(process);
            if (partitionIndex != -1) {
                logMemoryStatus(memoryLog, memoryPartitions); 
                logExecutionStatus(executionLog, process.pid, "NEW", "READY");
                executionQueue.push(process); // add the process to the priority queue 
            } else {
                readyQueue.push(process); // requeue if memory is not available
                break; // exit loop if no memory is available
            }
        }

        // handle execution (CPU utilization)
        if (!executionQueue.empty()) {
            Process process = executionQueue.top();
            executionQueue.pop();

            // log the transition from `READY -> RUNNING`
            if (process.state != 1 /* RUNNING */) {
                logExecutionStatus(executionLog, process.pid, "READY", "RUNNING");
                // update state to RUNNING
                process.state = 1; 
            }

            process.remainingCPUTime--;
            currentTime++;

            // handle the I/O 
            if (process.ioFrequency > 0 &&
                (process.totalCPUTime - process.remainingCPUTime) % process.ioFrequency == 0 &&
                process.remainingCPUTime > 0) {
                logExecutionStatus(executionLog, process.pid, "RUNNING", "WAITING");

                // change process state to waiting 
                int ioEndTime = static_cast<int>(currentTime) + process.ioDuration;
                waitingProcesses.push_back({process, ioEndTime});
            } else if (process.remainingCPUTime > 0) {
                executionQueue.push(process); // add back to the exec queue if not compeleted (just like in previous implementation)
            } else {
                logExecutionStatus(executionLog, process.pid, "RUNNING", "TERMINATED");
                deallocateMemory(process); // free unused memory
                logMemoryStatus(memoryLog, memoryPartitions); // log the decollation 
            }
        } else if (!waitingProcesses.empty()) {
            // if no process is running, advance to the next I/O completion
            int nextIOCompletionTime = waitingProcesses.front().second;
            for (const auto &pair : waitingProcesses) {
                nextIOCompletionTime = std::min(nextIOCompletionTime, pair.second);
            }
            currentTime = static_cast<uint16_t>(std::max(static_cast<int>(currentTime), nextIOCompletionTime));
        } else if (!readyQueue.empty()) {
            // if no processes are ready or waiting, skip to the next process
            currentTime = static_cast<uint16_t>(std::max(static_cast<int>(currentTime), static_cast<int>(readyQueue.front().arrivalTime)));
        }

        for (auto it = waitingProcesses.begin(); it != waitingProcesses.end();) {
            auto &waitingProcess = it->first;
            int ioEndTime = it->second;

            if (currentTime >= ioEndTime) {
                // I/O completed transition to READY State 
                if (waitingProcess.state != 3 /* READY */) {
                    logExecutionStatus(executionLog, waitingProcess.pid, "WAITING", "READY");
                    waitingProcess.state = 3; // udpate state to READY
                }
                executionQueue.push(waitingProcess);
                it = waitingProcesses.erase(it);
            } else {
                ++it;
            }
        }
    }
}

// bonus : RR scheduler w 100ns timeout
void runRoundRobinScheduler(std::queue<Process> &readyQueue, std::ofstream &executionLog, std::ofstream &memoryLog) {
    std::vector<std::pair<Process, int>> waitingProcesses; // a pair used for processes + I/O end time
    std::queue<Process> executionQueue; // a circular queue used for RR
    const int timeQuantum = 100; 

    while (!readyQueue.empty() || !waitingProcesses.empty() || !executionQueue.empty()) {
        while (!readyQueue.empty() && readyQueue.front().arrivalTime <= currentTime) {
            Process process = readyQueue.front();
            readyQueue.pop();

            // attempt to allocate memory
            int partitionIndex = allocateMemory(process);
            if (partitionIndex != -1) {
                logMemoryStatus(memoryLog, memoryPartitions); // log allocation
                logExecutionStatus(executionLog, process.pid, "NEW", "READY"); // log state change
                executionQueue.push(process); // add process to RR queue
                readyQueue.push(process); // Requeue if memory not available
                break; // Exit allocation loop if no memory is available
            }
        }

        if (!executionQueue.empty()) {
            Process process = executionQueue.front();
            executionQueue.pop();

            // Log `READY -> RUNNING` transition if needed
            if (process.state != 1 /* RUNNING */) {
                logExecutionStatus(executionLog, process.pid, "READY", "RUNNING");
                process.state = 1; // Update state to RUNNING
            }

            int timeUsed = 0;
            while (timeUsed < timeQuantum && process.remainingCPUTime > 0) {
                // simulate 1 ms of CPU time
                process.remainingCPUTime--;
                timeUsed++;
                currentTime++;

                // check for pending IO
                if (process.ioFrequency > 0 &&
                    (process.totalCPUTime - process.remainingCPUTime) % process.ioFrequency == 0 &&
                    process.remainingCPUTime > 0) {
                    logExecutionStatus(executionLog, process.pid, "RUNNING", "WAITING");

                    // change state to "WAITING"
                    int ioEndTime = currentTime + process.ioDuration;
                    waitingProcesses.push_back({process, ioEndTime});
                    break; // stop processing this process
                }
            }

            // if the process is not finished, requeue or terminate
            if (process.remainingCPUTime > 0 && timeUsed == timeQuantum) {
                logExecutionStatus(executionLog, process.pid, "RUNNING", "READY");
                executionQueue.push(process); // Requeue for the next round
            } else if (process.remainingCPUTime == 0) {
                logExecutionStatus(executionLog, process.pid, "RUNNING", "TERMINATED");
                deallocateMemory(process); // Free memory
                logMemoryStatus(memoryLog, memoryPartitions); // Log memory deallocation
            }
        } else if (!waitingProcesses.empty()) {
            int nextIOCompletionTime = waitingProcesses.front().second;
            for (const auto &pair : waitingProcesses) {
                nextIOCompletionTime = std::min(nextIOCompletionTime, pair.second);
            }
            currentTime = std::max(currentTime, static_cast<uint16_t>(nextIOCompletionTime));
        } else if (!readyQueue.empty()) {
            // If no processes are ready or waiting, skip to the next process arrival
            currentTime = std::max(currentTime, static_cast<uint16_t>(readyQueue.front().arrivalTime));
        }

        for (auto it = waitingProcesses.begin(); it != waitingProcesses.end();) {
            auto &waitingProcess = it->first;
            int ioEndTime = it->second;

            if (currentTime >= ioEndTime) {
                // I/O completed transition back to READY state
                if (waitingProcess.state != 3 /* READY */) {
                    logExecutionStatus(executionLog, waitingProcess.pid, "WAITING", "READY");
                    waitingProcess.state = 3; // Update to READY state
                }
                // add back to the execution queue 
                executionQueue.push(waitingProcess); 
                // remove the process from waiting list
                it = waitingProcesses.erase(it);    
            } else {
                ++it;
            }
        }
    }
}


std::queue<Process> readInputData(const std::string &filename) {
    std::ifstream inputFile(filename);
    std::queue<Process> processes;

    if (!inputFile) {
        std::cerr << "Error opening input file: " << filename << std::endl;
        return processes;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream ss(line);
        Process process;
        std::string token;

        // read each part of the trace line
        if (std::getline(ss, token, ',')) process.pid = std::stoi(token);
        if (std::getline(ss, token, ',')) process.memorySize = std::stoi(token);
        if (std::getline(ss, token, ',')) process.arrivalTime = std::stoi(token);
        if (std::getline(ss, token, ',')) process.totalCPUTime = std::stoi(token);
        if (std::getline(ss, token, ',')) process.ioFrequency = std::stoi(token);
        if (std::getline(ss, token, ',')) process.ioDuration = std::stoi(token);

        // initialize the rest of memebers in the process struct 
        process.remainingCPUTime = process.totalCPUTime;
        process.state = 0; // new state 
        process.memoryPartition = -1; // not allocated yet 

        // add process to the queue
        processes.push(process);
    }

    inputFile.close();
    return processes;
}

int main(int argc, char *argv[]) {
    if (argc < 3) { 
        std::cerr << "Usage: " << argv[0] << " <input_file> <scheduler>\n";
        return 1;
    }

    std::string inputFile = argv[1]; // argument to take the input type
    std::string scheduler = argv[2];  //argukment to take the scheduler type

    // Hardcoded output filenames
    const std::string executionFile = "execution_101259149_101260364.txt";
    const std::string memoryFile = "memory_status_101259149_101260364.txt";

    // Open output files
    std::ofstream executionLog(executionFile);
    std::ofstream memoryLog(memoryFile);

    if (!executionLog || !memoryLog) {
        std::cerr << "Error opening output files." << std::endl;
        return 1;
    }

    // Read the input trace
    std::queue<Process> processes = readInputData(inputFile);

    // Log the initial state of the memory (first line of mem status)
    logMemoryStatus(memoryLog, memoryPartitions);

    // Logic for running a specific scheduler
    if (scheduler == "FCFS") {
        runScheduler(processes, executionLog, memoryLog);
    } else if (scheduler == "EP") {
        runPriorityScheduler(processes, executionLog, memoryLog);
    } else if (scheduler == "RR") {
        runRoundRobinScheduler(processes, executionLog, memoryLog);
    } else {
        std::cerr << "Invalid scheduler type. Use 'FCFS', 'EP', or 'RR'." << std::endl;
        return 1;
    }

    // Close files used for logging
    executionLog.close();
    memoryLog.close();

    return 0;
}
