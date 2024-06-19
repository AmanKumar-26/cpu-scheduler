#include <bits/stdc++.h>

using namespace std;
map<int,string>mp;
struct Process {
    int id;
    int arrival_time;
    int burst_time;
    int priority;
    int remaining_time;  
    int start_time[20];  
    int wait_time;
    int finish_time;
    int turnaround_time;
};


void FCFS(vector<Process> processes) {
    int n = processes.size();
    vector<int> wait_time(n), trn_ard_tm(n);
    int total_wait_time = 0, total_turn_around_time = 0;

    sort(processes.begin(), processes.end(), [](const Process &a, const Process &b) {
        return a.arrival_time < b.arrival_time;
    });

    wait_time[0] = 0;
    trn_ard_tm[0] = processes[0].burst_time;

    for (int i = 1; i < n; ++i) {
        wait_time[i] = trn_ard_tm[i - 1] + processes[i - 1].arrival_time - processes[i].arrival_time;
        if (wait_time[i] < 0) wait_time[i] = 0;
        trn_ard_tm[i] = wait_time[i] + processes[i].burst_time;
    }

    for (int i = 0; i < n; ++i) {
        total_wait_time += wait_time[i];
        
        total_turn_around_time += trn_ard_tm[i];
    }
    mp[(double)total_wait_time / n]=" FCFS ";

    cout << "FCFS Average Waiting Time: " << (double)total_wait_time / n << endl;
    cout << "FCFS Average Turnaround Time: " << (double)total_turn_around_time / n << endl;
}


void SJF(vector<Process> processes) {
    int n = processes.size();
    vector<int> wait_time(n), trn_ard_tm(n);
    int total_wait_time = 0, total_turn_around_time = 0;

    
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
            trn_ard_tm[idx] = current_time - processes[idx].arrival_time;
            completed[idx] = true;
            completed_count++;
        } else {
            current_time++;
        }
    }

    for (int i = 0; i < n; ++i) {
        total_wait_time += wait_time[i];
        total_turn_around_time += trn_ard_tm[i];
    }
 mp[(double)total_wait_time / n]=" SJF ";
    cout << "SJF Average Waiting Time: " << (double)total_wait_time / n << endl;
    cout << "SJF Average Turnaround Time: " << (double)total_turn_around_time / n << endl;
}


void PriorityScheduling(vector<Process> processes) {
    int n = processes.size();
    vector<int> wait_time(n), trn_ard_tm(n);
    int total_wait_time = 0, total_turn_around_time = 0;

    
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
            trn_ard_tm[idx] = current_time - processes[idx].arrival_time;
            completed[idx] = true;
            completed_count++;
        } else {
            current_time++;
        }
    }

    for (int i = 0; i < n; ++i) {
        total_wait_time += wait_time[i];
        total_turn_around_time += trn_ard_tm[i];
    }
 mp[(double)total_wait_time / n]="Priority Scheduling ";
    cout << "Priority Scheduling Average Waiting Time: " << (double)total_wait_time / n << endl;
    cout << "Priority Scheduling Average Turnaround Time: " << (double)total_turn_around_time / n << endl;
}


void RoundRobin(vector<Process> processes, int quantum) {
    int n = processes.size();
    vector<int> wait_time(n), trn_ard_tm(n);
    int total_wait_time = 0, total_turn_around_time = 0;

    int c = n;
    float time = 0, mini = INT_MAX;
    vector<int> burst_time(n), arrival_time(n);
    int s[n][20]; 

    for (int i = 0; i < n; i++) {
        burst_time[i] = processes[i].burst_time;
        arrival_time[i] = processes[i].arrival_time;
        processes[i].remaining_time = processes[i].burst_time;

        for (int j = 0; j < 20; j++) {
            s[i][j] = -1;
        }
    }

    int tot_wt = 0, tot_tat = 0;
    bool flag = false;

    while (c != 0) {
        mini = INT_MAX;
        flag = false;

        for (int i = 0; i < n; i++) {
            float p = time + 0.1;
            if (arrival_time[i] <= p && mini > arrival_time[i] && processes[i].remaining_time > 0) {
                mini = arrival_time[i];
                flag = true;
            }
        }

        if (!flag) {
            time++;
            continue;
        }

        int index = -1;
        for (int i = 0; i < n; i++) {
            float p = time + 0.1;
            if (arrival_time[i] <= p && mini == arrival_time[i] && processes[i].remaining_time > 0) {
                index = i;
                break;
            }
        }

        int j = 0;
        while (s[index][j] != -1) {
            j++;
        }

        if (s[index][j] == -1) {
            s[index][j] = time;
            processes[index].start_time[j] = time;
        }

        if (processes[index].remaining_time <= quantum) {
            time += processes[index].remaining_time;
            processes[index].remaining_time = 0;
        } else {
            time += quantum;
            processes[index].remaining_time -= quantum;
        }

        if (processes[index].remaining_time > 0) {
            arrival_time[index] = time + 0.1;
        }

        if (processes[index].remaining_time == 0) {
            c--;
            processes[index].finish_time = time;
            processes[index].wait_time = processes[index].finish_time - processes[index].arrival_time - processes[index].burst_time;
            tot_wt += processes[index].wait_time;
            processes[index].turnaround_time = processes[index].burst_time + processes[index].wait_time;
            tot_tat += processes[index].turnaround_time;
        }
    }

    for (int i = 0; i < n; i++) {
        int j = 0;
        int v = 0;
        while (s[i][j] != -1) {
            
            j++;
            v += 3;
        }
        while (v != 40) {
          
            v += 1;
        }
       
    }

    double avg_wt = tot_wt / static_cast<double>(n);
    double avg_tat = tot_tat / static_cast<double>(n);
mp[avg_wt]= "Round Robin ";
    cout << "Round Robin Average Waiting Time: " << avg_wt << endl;
    cout << "Round Robin Average Turnaround Time: " << avg_tat << endl;
}

int main() {
    int n, quantum;
    cin >> n;

    vector<Process> processes(n);

    for (int i = 0; i < n; ++i) {
        cin >> processes[i].arrival_time;
        processes[i].id = i + 1;
    }

    for (int i = 0; i < n; ++i) {
        cin >> processes[i].burst_time;
    }

    for (int i = 0; i < n; ++i) {
        cin >> processes[i].priority;
    }
    cin >> quantum;

    cout << endl;
    FCFS(processes);
    cout << endl;
    SJF(processes);
    cout << endl;
    PriorityScheduling(processes);
    cout << endl;
    RoundRobin(processes, quantum);
    cout<<endl;
    cout<<endl;
    for(auto it : mp){
cout<<"Best Algorithim for given constraints :"<<endl;
cout<<"             "<<it.second<<"        "<<endl; 
break;
    }
      

    return 0;
}
