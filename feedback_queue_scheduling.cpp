#include <iostream>
#include <vector>
#include <queue>
using namespace std;

class Queue {
private:
    int front = -1;
    int size;
    int rear = -1;
    string* queue;

public:
    Queue(int size)
    {
        this->size = size;
        this->queue = new string[size];
    }

    bool isFull() {
        return front == 0 && rear == size - 1;
    }

    bool isEmpty() {
        return front == -1;
    }

    void enqueue(string item) {
        if (isFull()) {
            return; // Queue is full, cannot enqueue.
        }
        if (front == -1) {
            front = 0;
        }
        rear++;
        queue[rear] = item;
    }

    string dequeue() {
        string item;
        if (isEmpty()) {
            return " "; // Queue is empty.
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

    string getFront() {
        if (isEmpty()) {
            return " "; // Queue is empty.
        }
        return queue[front];
    }
};

int main()
{
    // Initialize queues for different scheduling queues and a vector to store time points.
    Queue q1(10);    //system queue
    Queue q2(10);   //interactive queue
    Queue q3(10);   //batch queue
    vector<int> gt;  //used for calculating wt, tat,ct

    cout << endl;
    cout << "Queue-1 and Queue-2 uses Round Robin Algorithm" << endl;
    cout << "Queue uses FCFS Algorithm" << endl;

    int n, t1, t2;
    double totalwt = 0, totaltat = 0;

    // Taking user input
    cout << "Enter the number of processes: ";
    cin >> n;
    int at[n], bt[n], rt[n], ct[n], wt[n], tat[n]; // arrival, burst, remaining, completion, waiting, turn around time
    string pro[n]; //for storing name of process

    cout << "Enter the time quantum for Queue 1: ";
    cin >> t1;
    cout << "Enter the time quantum for Queue 2: ";
    cin >> t2;
    cin.ignore(); // Clear the newline character from the input buffer.

    // Collect process information (arrival time, burst time, and process name) while sorting processes by arrival time.
    for (int i = 0; i < n; i++)
    {
        cout << "Enter the arrival time of process -  " << (i + 1) << ": ";
        cin >> at[i];
        cout << "Enter the burst time of process -  " << (i + 1) << ": ";
        cin >> bt[i];
        rt[i] = bt[i]; //remaining time
        cin.ignore(); // Clear the newline character from the input buffer.
        cout << "Enter the process name:(P1,P2...) -  ";
        getline(cin, pro[i]); // saving name we use this array and we insert the data into the array
    }

    // Sort processes by arrival time.
    for (int i = 0; i <= n - 2; i++)
    {
        for (int j = i + 1; j <= n - 1; j++)
        {
            if (at[i] > at[j])
            {
                // Swap process attributes to sort by arrival time.
                swap(at[i], at[j]);
                swap(bt[i], bt[j]);
                swap(rt[i], rt[j]);
                swap(pro[i], pro[j]);
            }
        }
    }

    int j = -1; //index of execution
    gt.push_back(at[0]); // Initialize time points with the first process arrival time.

    // Simulate scheduling for queue 1 (Round Robin).
    for (int i = 0; i < n; i++)
    {
        j++;
        if (rt[i] <= t1)  //if the remaining time is less than eqal to time quantum, run the process till time quantum
        {
            gt.push_back(gt[j] + rt[i]);
            rt[i] = 0;
            ct[i] = gt[j + 1];
            q1.enqueue(pro[i]);
            cout<<pro[i]<<" | ";
            cout<<gt[i]<<" | ";
        }
        else
        {
            // Execute the process for a time quantum and handle preemption.
            gt.push_back(gt[j] + t1);
            rt[i] -= t1;
            bool runloop = true; // check if the loop has to run or not (if )
            if (i < n - 1 && runloop)
            {
                while (rt[i] > 0 && gt[j + 1] < at[i + 1])
                {
                    // Continue executing until the next process arrives or the current process completes.
                    gt.push_back(gt[j + 1] + t1);
                    rt[i] -= t1;
                    j++;
                    q1.enqueue(pro[i]);
                    cout<<pro[i]<<" | ";
                    cout<<gt[i]<<" | ";
                }
                runloop = false; // Set runloop to false after the while loop.
            }
            q1.enqueue(pro[i]);
            cout<<pro[i]<<" | ";
            cout<<gt[i]<<" | ";
        }
    }

    // Place remaining processes in queue 2.
    for (int i = 0; i < n; i++)
    {
        if (rt[i] > 0)
        {
            q2.enqueue(pro[i]);
            cout<<pro[i]<<" | ";
            cout<<gt[i]<<" | ";
        }
    }
    cout << endl;
    // Print the processes in queue 1.
    cout << "Queue 1: ";
    while (!q1.isEmpty())
    {
        cout << q1.getFront() << " ";
        q1.dequeue();
    }
    cout << endl;

    // Simulate scheduling for queue 2 (Round Robin).
    for (int i = 0; i < n; i++)
    {
        if (rt[i] > 0)
        {
            j++;
            if (rt[i] <= t2)
            {
                // Execute the process if its remaining time is less than or equal to the time quantum.
                gt.push_back(gt[j] + rt[i]);
                rt[i] = 0;
                ct[i] = gt[j + 1];
            }
            else
            {
                // Execute the process for a time quantum and handle preemption.
                gt.push_back(gt[j] + t2);
                rt[i] -= t2;
            }
        }
    }

    // Print the processes in queue 2.
    cout << "Queue 2: ";
    while (!q2.isEmpty())
    {
        cout << q2.getFront() << " ";
        q2.dequeue();
    }
    cout << endl;

    // Place remaining processes in queue 3.
    for (int i = 0; i < n; i++)
    {
        if (rt[i] > 0)
        {
            q3.enqueue(pro[i]);
            cout<<pro[i]<<" | ";
            cout<<gt[i]<<" | ";
        }
    }

    // Simulate scheduling for queue 3 (FCFS).
    for (int i = 0; i < n; i++)
    {
        if (rt[i] > 0)
        {
            j++;
            gt.push_back(gt[j] + rt[i]);
            rt[i] = 0;
            ct[i] = gt[j + 1];
        }
    }

    // Print the processes in queue 3.
    cout << "Queue 3: ";
    while (!q3.isEmpty())
    {
        cout << q3.getFront() << " ";
        q3.dequeue();
    }
    cout << endl;

    // Calculate turnaround times and waiting times.
    for (int i = 0; i < n; i++)
    {
        tat[i] = ct[i] - at[i];
        wt[i] = tat[i] - bt[i];
    }

    cout << endl;
    // Calculate completion times (CT)
    for (int i = 0; i < n; i++)
    {
        ct[i] = at[i] + tat[i];
    }

    // Print a table of process information (BT, CT, TAT, WT)
    cout << "Process\tBT\tCT\tTAT\tWT" << endl;
    for (int i = 0; i < n; i++)
    {
        cout << pro[i] << "\t" << bt[i] << "\t" << ct[i] << "\t" << tat[i] << "\t" << wt[i] << endl;
    }

    // Calculate and print average turnaround time and waiting time.
    for (int i = 0; i < n; i++)
    {
        totaltat += tat[i];
        totalwt += wt[i];
    }
    cout << endl;
    cout << "Average turn around time is " << totaltat / n << endl;
    cout << "Average waiting time is " << totalwt / n << endl;
    
    return 0;
}
