#include <bits/stdc++.h>

using namespace std;

struct Process {
    int id;
    int arrival_time;
    int burst_time;
    int priority;
    int remaining_time;  // For Round Robin
};

// Function to calculate average times for FCFS
void FCFS(vector<Process> processes) {
    int n = processes.size();
    vector<int> wait_time(n), turn_around_time(n);
    int total_wait_time = 0, total_turn_around_time = 0;

    // Sorting by arrival time
    sort(processes.begin(), processes.end(), [](const Process &a, const Process &b) {
        return a.arrival_time < b.arrival_time;
    });

    wait_time[0] = 0;
    turn_around_time[0] = processes[0].burst_time;

    for (int i = 1; i < n; ++i) {
        wait_time[i] = turn_around_time[i-1] + processes[i-1].arrival_time - processes[i].arrival_time;
        if (wait_time[i] < 0) wait_time[i] = 0;
        turn_around_time[i] = wait_time[i] + processes[i].burst_time;
    }

    for (int i = 0; i < n; ++i) {
        total_wait_time += wait_time[i];
        total_turn_around_time += turn_around_time[i];
    }

    cout << "FCFS Average Waiting Time: " << (double)total_wait_time / n << endl;
    cout << "FCFS Average Turnaround Time: " << (double)total_turn_around_time / n << endl;
}

// Function to calculate average times for SJF
void SJF(vector<Process> processes) {
    int n = processes.size();
    vector<int> wait_time(n), turn_around_time(n);
    int total_wait_time = 0, total_turn_around_time = 0;

    // Sorting by arrival time
    sort(processes.begin(), processes.end(), [](const Process &a, const Process &b) {
        return a.arrival_time < b.arrival_time;
    });

    vector<bool> completed(n, false);
    int current_time = 0, completed_count = 0;

    while (completed_count < n) {
        int idx = -1;
        int min_burst_time = INT_MAX;
        for (int i = 0; i < n; ++i) {
            if (!completed[i] && processes[i].arrival_time <= current_time && processes[i].burst_time < min_burst_time) {
                min_burst_time = processes[i].burst_time;
                idx = i;
            }
        }

        if (idx != -1) {
            current_time += processes[idx].burst_time;
            wait_time[idx] = current_time - processes[idx].arrival_time - processes[idx].burst_time;
            turn_around_time[idx] = current_time - processes[idx].arrival_time;
            completed[idx] = true;
            completed_count++;
        } else {
            current_time++;
        }
    }

    for (int i = 0; i < n; ++i) {
        total_wait_time += wait_time[i];
        total_turn_around_time += turn_around_time[i];
    }

    cout << "SJF Average Waiting Time: " << (double)total_wait_time / n << endl;
    cout << "SJF Average Turnaround Time: " << (double)total_turn_around_time / n << endl;
}

// Function to calculate average times for Priority Scheduling
void PriorityScheduling(vector<Process> processes) {
    int n = processes.size();
    vector<int> wait_time(n), turn_around_time(n);
    int total_wait_time = 0, total_turn_around_time = 0;

    // Sorting by arrival time, then by priority
    sort(processes.begin(), processes.end(), [](const Process &a, const Process &b) {
        if (a.arrival_time == b.arrival_time)
            return a.priority < b.priority;
        return a.arrival_time < b.arrival_time;
    });

    vector<bool> completed(n, false);
    int current_time = 0, completed_count = 0;

    while (completed_count < n) {
        int idx = -1;
        int highest_priority = INT_MAX;
        for (int i = 0; i < n; ++i) {
            if (!completed[i] && processes[i].arrival_time <= current_time && processes[i].priority < highest_priority) {
                highest_priority = processes[i].priority;
                idx = i;
            }
        }

        if (idx != -1) {
            current_time += processes[idx].burst_time;
            wait_time[idx] = current_time - processes[idx].arrival_time - processes[idx].burst_time;
            turn_around_time[idx] = current_time - processes[idx].arrival_time;
            completed[idx] = true;
            completed_count++;
        } else {
            current_time++;
        }
    }

    for (int i = 0; i < n; ++i) {
        total_wait_time += wait_time[i];
        total_turn_around_time += turn_around_time[i];
    }

    cout << "Priority Scheduling Average Waiting Time: " << (double)total_wait_time / n << endl;
    cout << "Priority Scheduling Average Turnaround Time: " << (double)total_turn_around_time / n << endl;
}

// Function to calculate average times for Round Robin
void RoundRobin(vector<Process> processes, int quantum) {
    int n = processes.size();
    vector<int> wait_time(n, 0), turn_around_time(n, 0);
    int total_wait_time = 0, total_turn_around_time = 0;
    queue<int> q;
    int current_time = 0;

    sort(processes.begin(), processes.end(), [](const Process &a, const Process &b) {
        return a.arrival_time < b.arrival_time;
    });

    // Initial load of processes based on their arrival times
    for (int i = 0; i < n; ++i) {
        processes[i].remaining_time = processes[i].burst_time;
    }

    int index = 0;
    while (index < n && processes[index].arrival_time <= current_time) {
        q.push(index++);
    }

    while (!q.empty()) {
        int i = q.front();
        q.pop();

        if (processes[i].remaining_time > quantum) {
            current_time += quantum;
            processes[i].remaining_time -= quantum;
        } else {
            current_time += processes[i].remaining_time;
            processes[i].remaining_time = 0;
            wait_time[i] = current_time - processes[i].arrival_time - processes[i].burst_time;
            turn_around_time[i] = current_time - processes[i].arrival_time;
        }

        // Add newly arrived processes to the queue
        while (index < n && processes[index].arrival_time <= current_time) {
            q.push(index++);
        }

        // Re-add the current process to the queue if it has remaining burst time
        if (processes[i].remaining_time > 0) {
            q.push(i);
        } else if (q.empty() && index < n) {
            // If the queue is empty and there are still processes to arrive
            current_time = processes[index].arrival_time;
            q.push(index++);
        }
    }

    for (int i = 0; i < n; ++i) {
        total_wait_time += wait_time[i];
        total_turn_around_time += turn_around_time[i];
    }

    cout << "Round Robin Average Waiting Time: " << (double)total_wait_time / n << endl;
    cout << "Round Robin Average Turnaround Time: " << (double)total_turn_around_time / n << endl;
}


int main()
{
    ifstream inputFile("input.txt");
    if (!inputFile) {
        cerr << "Error opening input file." << endl;
        return 1;
    }

    int n;
    inputFile >> n;

    vector<Process> processes(n);
    for (int i = 0; i < n; ++i) {
        processes[i].id = i;
        inputFile >> processes[i].arrival_time >> processes[i].burst_time >> processes[i].priority;
    }

    int timeQuantum;
    inputFile >> timeQuantum;

    inputFile.close();

    FCFS(processes);
    SJF(processes);
    PriorityScheduling(processes);
    RoundRobin(processes, timeQuantum);

    return 0;
}
