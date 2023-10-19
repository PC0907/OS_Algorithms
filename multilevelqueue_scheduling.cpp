#include <iostream>
#include <queue>
#define INT_MAX 100
using namespace std;

class Queue {
private:
    int front = -1;
    int size;
    int rear = -1;
    int* queue;

public:
    Queue(int size)
    {
        this->size = size;
        this->queue = new int[size];
    }

    bool isFull() {
        return front == 0 && rear == size - 1;
    }

    bool isEmpty() {
        return front == -1;
    }

    void enqueue(int item) {
        if (isFull()) {
            return; // Queue is full, cannot enqueue.
        }
        if (front == -1) {
            front = 0;
        }
        rear++;
        queue[rear] = item;
    }

    int dequeue() {
        int item;
        if (isEmpty()) {
            return -1; // Queue is empty.
        } else {
            item = queue[front];
            if (front >= rear) {
                front = rear = -1;
            } else {
                front++;
            }
            return item;
        }
    }

    int getFront() {
        if (isEmpty()) {
            return -1; // Queue is empty.
        }
        return queue[front];
    }
};

//initializing structures
int currentTime = 0;
Queue rr(10);       //queue for round robin scheduling
int isVisitedr[10];  //to check visited processes in round robin scheduling
int isVisiteds[10];  //to check visited processes in sjf scheduling
int sjf[100];
int sjfIndex = 0;
int priority[2]; //to store the type of processes in the queue-- 0 for system processes , 1 for interactive processes, and 2 for batch processes.
int gantt_chart[100];

int currentIndex(int AT[], int n) {  //to find the index of the job to execute.
    int cindex = 0;
    for (int i = 0; i <= currentTime; i++)
    {
        if (i == n)
        {
            return n;
        }
        if (AT[i] <= currentTime)
        {
            cindex = i;
        }
    }
    return cindex + 1;
}

int minsjf(int BT[])  //o find the job with the minimum remaining burst time. returns the index of the jon with minimum burst
{
    int minjob = INT_MAX;
    int minjobIndex = -1;
    for (int i = 0; i < sjfIndex; i++) {
        if (minjob > BT[sjf[i]] && BT[sjf[i]] > 0) {
            minjob = BT[sjf[i]];
            minjobIndex = sjf[i];
        }
    }
    return minjobIndex;
}

bool highestPriority(int n, int P[], int BT[])  //to find the job with the highest priority. returns true if there are more jobs to execute based on priority
{
    int priorityVal = 100;
    bool iscontinue = false;
    for (int i = 0; i < n; i++) {
        if (priorityVal > P[i] && BT[i] > 0) {
            priorityVal = P[i];
            priority[0] = priorityVal;
            priority[1] = i;
            iscontinue = true;
        }
    }
    return iscontinue;
}

void fcfs(int p, int BT[]) {
    currentTime++;
    gantt_chart[currentTime] = p;
    BT[p]--;
}

void roundRobin(int p, int quantumTime, int n, int BT[], int AT[], int P[])
{
    int temp = p;
    int k;
    if (rr.isEmpty()) {
        k = min(quantumTime, BT[p]);
        for (int i = 0; i < k; i++)
        {
            currentTime++;
            gantt_chart[currentTime] = p;
            BT[p]--;
        }
    } 
    else
    {
        temp = rr.getFront();
        rr.dequeue();
        k = quantumTime;
        if (BT[temp] < quantumTime)
        {
            k = BT[temp];
        }
        for (int i = 0; i < k; i++)
        {
            currentTime++;
            gantt_chart[currentTime] = temp;
            BT[temp]--;
        }
    }
    for (int i = temp + 1; i <= currentTime; i++)
    {
        if (i < n && BT[i] > 0 && isVisitedr[i] == -1 && P[i] == 1)
        {
            rr.enqueue(i);
            isVisitedr[i] = 1;
        }
    }
    if (BT[temp] != 0)  //if the process has not bee completed, enqueue it
    {
        rr.enqueue(temp);
    }
}

void SJF(int p, int n, int BT[], int AT[])
{
    if (isVisiteds[p] == -1)  //if the process is still waiting 
    {
        sjf[sjfIndex] = p;
        sjfIndex++;
        isVisiteds[p] = 1;
    }
    int minjob = minsjf(BT);
    if (minjob != -1)
    {
        BT[minjob]--;
        currentTime++;
        gantt_chart[currentTime] = minjob;
    }
}

void completion(int gantt_chart[], int CT[], int n) {
    CT[gantt_chart[currentTime]] = currentTime;
    for (int i = currentTime; i > 0; i--) {
        if (gantt_chart[i] != gantt_chart[i - 1] && CT[gantt_chart[i - 1]] == -1) {
            CT[gantt_chart[i - 1]] = i - 1;
        }
    }
    cout << "Completion time: ";
    for (int i = 0; i < n; i++) {
        cout << CT[i] << " ";
    }
    cout << endl;
}

void TATime(int CT[], int AT[], int TAT[], int n) {
    for (int i = 0; i < n; i++) {
        TAT[i] = CT[i] - AT[i];
    }
    float avg = 0;
    cout << "Turnaround time: ";
    for (int i = 0; i < n; i++) {
        cout << TAT[i] << " ";
        avg += TAT[i];
    }
    cout << endl;
    cout << "Average Turnaround time: " << avg / n << endl;
}

void WAT(int TAT[], int BT[], int WT[], int n) {
    for (int i = 0; i < n; i++) {
        WT[i] = TAT[i] - BT[i];
    }
    float avg = 0;
    cout << "Waiting time: ";
    for (int i = 0; i < n; i++) {
        cout << WT[i] << " ";
        avg += WT[i];
    }
    cout << endl;
    cout << "Average waiting time: " << avg / n << endl;
}

int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;
    int quantumTime;
    cout << "Enter time quantum: ";
    cin >> quantumTime;
    int AT[n];
    int BT[n];
    int remaining_BT[n];
    int P[n];
    int CT[n];
    int TAT[n];
    int WT[n];
    for (int i = 0; i < n; i++) 
    {
        CT[i] = -1;
        TAT[i] = -1;
        WT[i] = -1;
    }

    
    
    for (int i = 0; i < 10; i++) {
        isVisiteds[i] = -1;
        isVisitedr[i] = -1;
    }

    cout << "Enter arrival times: ";
    
    for (int i = 0; i < n; i++) {
        cin >> AT[i];
    }
    
    cout << "Enter Burst times: ";
    
    for (int i = 0; i < n; i++) {
        cin >> BT[i];
        remaining_BT[i] = BT[i];
    }
    
    cout << "Enter priority: ";
    
    for (int i = 0; i < n; i++) {
        cin >> P[i];
    }
    
    bool iscontinue = true;
    
    while (iscontinue) {
        int N = currentIndex(AT, n);  //to get the index of the next job to be executed.
        iscontinue = highestPriority(N, P, remaining_BT);
        int process = priority[1];
        switch (priority[0]) {
            case 1:
                roundRobin(process, quantumTime, n, remaining_BT, AT, P);
                break;
            case 2:
                SJF(process, n, remaining_BT, AT);
                break;
            case 3:
                fcfs(process, remaining_BT);
                break;
        }
    }
    
    //printing gantt chart
    for (int i = 0; i <= currentTime; i++)
    {
        cout << gantt_chart[i] << " ";
    }
    cout<<endl;
    
    //calculating completion time, turn around time and waiting time for all the processes
    completion(gantt_chart, CT, n);
    TATime(CT, AT, TAT, n);
    WAT(TAT, BT, WT, n);
    cout << "Process\tBurst Time\tArrival Time\tWaiting Time\tTurnaround Time" << endl;
    for (int i = 0; i < n; i++) {
        cout << (i + 1) << "\t" << BT[i] << "\t\t" << AT[i] << "\t\t" << WT[i] << "\t\t" << TAT[i] << endl;
    }
    return 0;
}
