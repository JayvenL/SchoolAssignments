/**
 * Definition of functions for SYSC4001 A1
 * 
 * Authors: Aj Donald 101259149, Jayven Larsen 101260364
 */

#include "interrupts.hpp"
int main()
{

    std::string vectorFileName, TraceFile; 

    std::cout << "Enter the vector table file name: "; 
    std::cin >> vectorFileName; 
    

    std::cout << "Enter the trace file name: ";
    std::cin >> TraceFile; 

    inputRead(TraceFile, vectorFileName);
    
    return 0;
}
