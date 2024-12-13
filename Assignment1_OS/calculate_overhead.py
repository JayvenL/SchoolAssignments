"""
SYSC4001 Assignment 1 

Python Overhead calculator and helper function 

Authors: Aj Donald 101259149, Jayven Larsen 101260364

"""


# events that count as overhead
overhead_events = [
    "Switch to Kernel Mode",
    "Save Context",
    "IRET",
    "Find vector",
    "Load address"
]

cpu_time = 0
io_time = 0
overhead_time = 0
syscall_time = 0
total_time = 0

with open('execution.txt', 'r') as file:
    for line in file:
        parts = line.strip().split(', ')
        if len(parts) == 3:
            time, duration, event = int(parts[0]), int(parts[1]), parts[2]
            total_time += duration

            if "CPU Execution" in event:
                cpu_time += duration
            elif "I/O Completed" in event:
                io_time += duration
            elif "SYSCALL" in event:
                syscall_time += duration 
            elif any(overhead_event in event for overhead_event in overhead_events):
                overhead_time += duration  

# ratios 
overhead_time += syscall_time  
overhead_ratio = (overhead_time / total_time) * 100 if total_time else 0
cpu_ratio = (cpu_time / total_time) * 100 if total_time else 0
io_ratio = (io_time / total_time) * 100 if total_time else 0
syscall_ratio = (syscall_time / total_time) * 100 if total_time else 0 


# results
print(f"Total Overhead Time: {overhead_time} ms")
print(f"Total CPU Time: {cpu_time} ms")
print(f"Total I/O Time: {io_time} ms")
print(f"Total Simulation Time: {total_time} ms")
print(f"Overhead Ratio: {overhead_ratio:.2f}%")
print(f"CPU Ratio: {cpu_ratio:.2f}%")
print(f"I/O Ratio: {io_ratio:.2f}%")
