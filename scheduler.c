
// Adam Pinkos
// CS4420 - Operating Systems
// Assignme nt 3
// September 16th, 2026




#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESSES 20
#define MAX_QUEUE 1000

// Stores the information for one process
typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int start_time;
    int end_time;
    int waiting_time;
} Process;


// Store the information for the Gantt chart
int gantt_pid[1000];
int gantt_start[1000];
int gantt_end[1000];
int gantt_count = 0;


// Add a process to the Gantt chart
void addGantt(int pid, int start, int end) {

    if (gantt_count >= 1000) {
        return;
    }

    gantt_pid[gantt_count] = pid;
    gantt_start[gantt_count] = start;
    gantt_end[gantt_count] = end;

    gantt_count++;
}


// Print the Gantt chart
void printGantt() {

    printf("\nGantt Chart:\n\n");

    for (int i = 0; i < gantt_count; i++) {

        if (gantt_pid[i] == -1) {
            printf("| IDLE  ");
        }
        else {
            printf("| P%-4d ", gantt_pid[i]);
        }
    }

    printf("|\n");

    // Print the start and end times
    for (int i = 0; i < gantt_count; i++) {
        printf("%-8d", gantt_start[i]);
    }

    if (gantt_count > 0) {
        printf("%d", gantt_end[gantt_count - 1]);
    }

    printf("\n");
}


// First come first serve
void FCFS(Process processes[], int count) {
    
    int time = 0; // current time 
    int completed = 0; //processes 

    // Keep running until every process is completed
    while (completed < count) {

        // no process has been selected yet
        int selected = -1;

        // Go through all of the processes
        for (int i = 0; i < count; i++) {

            if (processes[i].arrival_time <= time) {
                if (processes[i].remaining_time > 0) {

                    // Select the first available process
                    if (selected == -1) {
                        selected = i;
                    }
                }
            }
        }

        // If no process is ready
        //CPU is idle
        if (selected == -1) {


            printf("Time %d: idle\n", time);

            addGantt(-1, time, time + 1);

            // Move the simulation forward by 1 time unit
            time = time + 1;
        }

        else {

            // Save the time
            processes[selected].start_time = time;

            // Print which process was selected
            printf("Time %d: Process %d selected\n",
                   time, processes[selected].pid);

            while (processes[selected].remaining_time > 0) {


                processes[selected].remaining_time =
                    processes[selected].remaining_time - 1;

                time = time + 1;
            }

            processes[selected].end_time = time;

            addGantt(processes[selected].pid,
                     processes[selected].start_time,
                     processes[selected].end_time);

            // Print that the process finished
            printf("Time %d: Process %d finished\n",
                   time, processes[selected].pid);

            // Get the values 
            int end_time = processes[selected].end_time;
            int arrival_time = processes[selected].arrival_time;
            int burst_time = processes[selected].burst_time;

        


            processes[selected].waiting_time =
                end_time - arrival_time - burst_time;

            // Add 1 to the number of completed processes
            completed = completed + 1;
        }
    }

    
// Print 
    double total_waiting = 0;

    printf("\nResults for each process:\n");

    for (int i = 0; i < count; i++) {

        printf("Process %d | Arrival: %d | Start: %d | Running: %d | End: %d | Waiting: %d\n",
               processes[i].pid, processes[i].arrival_time, processes[i].start_time,
               processes[i].burst_time, processes[i].end_time, processes[i].waiting_time);

        total_waiting = total_waiting + processes[i].waiting_time;
    }

    double average_waiting = total_waiting / count;

    printf("\nAverage Waiting Time: %.2f\n", average_waiting);

    printGantt();

}




/// Shortest Job First (SJF)
void SJF(Process processes[], int count) {
    int time = 0;
    int completed = 0;


    while (completed < count) {
        int selected = -1;

        // Loop through all processes
        for (int i = 0; i < count; i++) {
            if (processes[i].arrival_time <= time) {
                if (processes[i].remaining_time > 0) {
                    if (selected == -1) {
                        selected = i;
                    } 
                    else {
                        // check if current process has a smaller burst time
                        if (processes[i].burst_time < processes[selected].burst_time) {
                            selected = i;
                        }
                    }
                }
            }
        }

        // Check if CPU is idle
        if (selected == -1) {
            printf("Time %d: idle\n", time);

            addGantt(-1, time, time + 1);

            time = time + 1;
        } 
        else {
            processes[selected].start_time = time;

            printf("Time %d: Process %d selected\n", time, processes[selected].pid);

            // Run process 1 unit at a time until remaining time is 0
            while (processes[selected].remaining_time > 0) {
                processes[selected].remaining_time = processes[selected].remaining_time - 1;
                time = time + 1;
            }

            processes[selected].end_time = time;

            addGantt(processes[selected].pid,
                     processes[selected].start_time,
                     processes[selected].end_time);

            printf("Time %d: Process %d finished\n", time, processes[selected].pid);

            // Calculate waiting time s
            int end_time = processes[selected].end_time;
            int arrival_time = processes[selected].arrival_time;
            int burst_time = processes[selected].burst_time;

            processes[selected].waiting_time = end_time - arrival_time - burst_time;

            completed = completed + 1;
        }
    }

  
// Print 
    double total_waiting = 0;

    printf("\nResults for each process:\n");

    for (int i = 0; i < count; i++) {

        printf("Process %d | Arrival: %d | Start: %d | Running: %d | End: %d | Waiting: %d\n",
               processes[i].pid, processes[i].arrival_time, processes[i].start_time,
               processes[i].burst_time, processes[i].end_time, processes[i].waiting_time);

        total_waiting = total_waiting + processes[i].waiting_time;
    }

    double average_waiting = total_waiting / count;

    printf("\nAverage Waiting Time: %.2f\n", average_waiting);

    printGantt();

}





// Round Robin
void RR(Process processes[], int count, int quantum) {

    // current time and completed processes
    int time = 0;
    int completed = 0;

    // ready queue to keep track of the front and rear
    int queue[MAX_QUEUE];
    int front = 0;
    int rear = 0;


    int added[MAX_PROCESSES] = {0};

    // Print the algorithm name and time quantum
    printf("\nRR (Time quantum = %d):\n", quantum);

    // Continue until every process has finished
    while (completed < count) {

        // Check every process to see if it has arrived
        for (int i = 0; i < count; i++) {

            // Only add a process if it has arrived and was not added before
            if (processes[i].arrival_time <= time &&
                added[i] == 0) {

                // Put the process at the back of the ready queue
                queue[rear] = i;
                rear++;

                // Mark the process as added so it is not added twice
                added[i] = 1;
            }
        }

        // Check if the ready queue is empty
        if (front == rear) {

            // No process is ready, so the CPU is idle
            printf("Time %d: idle\n", time);

            addGantt(-1, time, time + 1);

            // Move forward one time unit and check again
            time++;
            continue;
        }

        // Select the process at the front of the queue
        int selected = queue[front];
        front++;

        // Save the first time this process
        if (processes[selected].start_time == -1) {
            processes[selected].start_time = time;
        }

        // Remember when this turn started
        int start = time;

        // Print which process is currently selected
        printf("Time %d: Process %d selected\n",
               time, processes[selected].pid);

        // Count how much CPU time the process uses during this turn
        int running_time = 0;

        // Run until the quantum is used or the process finishes
        while (running_time < quantum &&
               processes[selected].remaining_time > 0) {

            // Subtract one unit from the process's remaining time
            processes[selected].remaining_time--;

            // Move the simulation and running time forward by one unit
            time++;
            running_time++;

            // Check if any new processes arrived while the CPU was running
            for (int i = 0; i < count; i++) {

                if (processes[i].arrival_time <= time &&
                    added[i] == 0) {

                    // Add the newly arrived process to the ready queue
                    queue[rear] = i;
                    rear++;

                    // Mark it as already added
                    added[i] = 1;
                }
            }
        }

        // Print when the process started and when it stopped and how long it ran
        printf("PID %d ran from %d to %d for %d ms\n",
               processes[selected].pid,
               start,
               time,
               running_time);

        addGantt(processes[selected].pid, start, time);

        // Check if the process has finished all its CPU time
        if (processes[selected].remaining_time == 0) {

            // Save the time e
            processes[selected].end_time = time;

            // Calculate waiting time
            processes[selected].waiting_time =
                processes[selected].end_time -
                processes[selected].arrival_time -
                processes[selected].burst_time;

            // Print that the process has finished
            printf("Time %d: Process %d finished\n",
                   time, processes[selected].pid);

            // Increase the number of completed processes
            completed++;
        }

        // If the process still needs CPU time
        else {

            // Put it at the back of the queue to wait for another turn
            queue[rear] = selected;
            rear++;
        }
    }


// Print 
    double total_waiting = 0;

    printf("\nResults for each process:\n");

    for (int i = 0; i < count; i++) {

        printf("Process %d | Arrival: %d | Start: %d | Running: %d | End: %d | Waiting: %d\n",
               processes[i].pid, processes[i].arrival_time, processes[i].start_time,
               processes[i].burst_time, processes[i].end_time, processes[i].waiting_time);

        total_waiting = total_waiting + processes[i].waiting_time;
    }

    double average_waiting = total_waiting / count;

    printf("\nAverage Waiting Time: %.2f\n", average_waiting);

    printGantt();

}



int main(int argc, char *argv[]) {

    Process processes[MAX_PROCESSES];

    int count;

    // Check 
    if (argc < 3) {

        printf("Usage: ./proj2 input_file FCFS|RR|SJF [time_quantum]\n");
        return 1;
    }

    // Open input file
    FILE *file = fopen(argv[1], "r");

    if (file == NULL) {

        printf("Could not open input file.\n");
        return 1;
    }

    // Read number of processes
    fscanf(file, "%d", &count);

    if (count > MAX_PROCESSES) {

        printf("Too many processes.\n");
        fclose(file);
        return 1;
    }

    // Read process information
    for (int i = 0; i < count; i++) {

        fscanf(file, "%d %d %d",
               &processes[i].pid,
               &processes[i].arrival_time,
               &processes[i].burst_time);

        processes[i].remaining_time =
            processes[i].burst_time;

        processes[i].start_time = -1;
        processes[i].end_time = -1;
        processes[i].waiting_time = 0;
    }

    fclose(file);


    // Start with an empty Gantt chart
    gantt_count = 0;


    // Run FCFS
    if (strcmp(argv[2], "FCFS") == 0) {

        FCFS(processes, count);
    }

    // Run SJF
    else if (strcmp(argv[2], "SJF") == 0) {

        SJF(processes, count);
    }

    // Run Round Robin
    else if (strcmp(argv[2], "RR") == 0) {

        if (argc < 4) {
            printf("Round Robin requires a time quantum.\n");
            return 1;
        }


        int quantum = atoi(argv[3]);
        if (quantum <= 0) {
            printf("Time quantum must be greater than 0.\n");
            return 1;
        }

        RR(processes, count, quantum);
    }

    else { 
        printf("Invalid scheduling algorithm.\n");
        return 1;
    }

    return 0;
}
