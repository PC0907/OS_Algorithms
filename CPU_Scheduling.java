import java.util.*;

class CPU_Scheduling
{
    public class Queue
    {
        int size;
        int queue[];
        int front = -1, rear = -1;

        public Queue(int size) {
            this.size = size;
            this.queue = new int[size];
        }

        public boolean isFull() {
            return front == 0 && rear == size - 1;
        }

        public boolean isEmpty() {
            return front == -1;
        }

        public void enqueue(int item) {
            if (isFull()) {
                System.out.print("Queue is full");
                return;
            } else if (front == -1) {
                front = 0;
            }
            rear = (rear + 1) % size; // Circular queue handling
            queue[rear] = item;
        }
        

        public int dequeue() {
            int item;
            if (isEmpty()) {
                System.out.println("Queue is Empty!");
                return (-1);
            } else {
                item = queue[front];
            }
            if (front >= rear) {
                front = -1;
                rear = -1;
            } else {
                front++;
            }
            return item;
        }
        public boolean contains(int item) {
            if (isEmpty()) {
                return false; // Queue is empty, so the item cannot be found.
            }
        
            for (int i = front; i <= rear; i++) {
                if (queue[i] == item) {
                    return true; // Item found in the queue.
                }
            }
        
            return false; // Item not found in the queue.
        }
    }

    
    public void preemptive_priority(char[] process, int[] burst, int[] arrival, int[] priority, int n) {
        int currentTime = 0;
        boolean[] completed = new boolean[n];
        
        for (int i = 0; i < n; i++) {
            completed[i] = false;
        }
    
        int[] remainingTime = Arrays.copyOf(burst, n);
        int[] finishTime = new int[n];
    
        System.out.println("Gantt Chart for Preemptive Priority Scheduling:");
        while (true) {
            int highestPriority = Integer.MAX_VALUE;
            int highestPriorityProcess = -1;
    
            boolean allCompleted = true;
    
            for (int i = 0; i < n; i++) {
                if (!completed[i] && arrival[i] <= currentTime && priority[i] < highestPriority) {
                    allCompleted = false;
                    highestPriority = priority[i];
                    highestPriorityProcess = i;
                }
            }
    
            if (allCompleted) {
                break;
            }
    
            currentTime++;
    
            if (highestPriorityProcess != -1) {
                remainingTime[highestPriorityProcess]--;
                System.out.print("| P" + highestPriorityProcess + " ");
                
                if (remainingTime[highestPriorityProcess] == 0) {
                    completed[highestPriorityProcess] = true;
                    finishTime[highestPriorityProcess] = currentTime;
                }
            } else {
                System.out.print("|   "); // Idle time (CPU is idle)
            }
        }
    
        System.out.println("|");
    
        int[] waitingTime = new int[n];
        int[] turnaroundTime = new int[n];
    
        double totalWaitingTime = 0;
        double totalTurnaroundTime = 0;
    
        for (int i = 0; i < n; i++) {
            turnaroundTime[i] = finishTime[i] - arrival[i];
            waitingTime[i] = turnaroundTime[i] - burst[i];
            totalWaitingTime += waitingTime[i];
            totalTurnaroundTime += turnaroundTime[i];
    
            System.out.println("Process P" + i + " - Waiting Time: " + waitingTime[i] + ", Turnaround Time: " + turnaroundTime[i]);
        }
    
        double avgWaitingTime = totalWaitingTime / n;
        double avgTurnaroundTime = totalTurnaroundTime / n;
    
        System.out.println("Average Waiting Time: " + avgWaitingTime);
        System.out.println("Average Turnaround Time: " + avgTurnaroundTime);
    }
    

    public void non_preemptive_sjf(char[] process, int[] burst, int[] arrival, int n) {
        int currentTime = 0;
        boolean[] completed = new boolean[n];
        int[] finishTime = new int[n];

        System.out.println("Gantt Chart for Non-Preemptive SJF:");
        while (true) {
            int shortestJob = -1;
            int minBurst = Integer.MAX_VALUE;

            boolean allCompleted = true;

            for (int i = 0; i < n; i++) {
                if (!completed[i] && arrival[i] <= currentTime && burst[i] < minBurst) {
                    allCompleted = false;
                    minBurst = burst[i];
                    shortestJob = i;
                }
            }

            if (allCompleted)
                break;

            currentTime += burst[shortestJob];
            completed[shortestJob] = true;
            finishTime[shortestJob] = currentTime;
            System.out.print("| P" + shortestJob + " ");
        }
        System.out.println("|");

        // Calculate and print waiting times and turnaround times
        int[] waitingTime = new int[n];
        int[] turnaroundTime = new int[n];

        double totalWaitingTime = 0;
        double totalTurnaroundTime = 0;

        for (int i = 0; i < n; i++) {
            turnaroundTime[i] = finishTime[i] - arrival[i];
            waitingTime[i] = turnaroundTime[i] - burst[i];
            totalWaitingTime += waitingTime[i];
            totalTurnaroundTime += turnaroundTime[i];

            System.out.println("Process P" + i + " - Waiting Time: " + waitingTime[i] + ", Turnaround Time: " + turnaroundTime[i]);
        }

        // Calculate and print average waiting time and average turnaround time
        double avgWaitingTime = totalWaitingTime / n;
        double avgTurnaroundTime = totalTurnaroundTime / n;

        System.out.println("Average Waiting Time: " + avgWaitingTime);
        System.out.println("Average Turnaround Time: " + avgTurnaroundTime);
    }

    public void round_robin(char[] process, int[] burst, int[] arrival, int n, int quantum) {
        int currentTime = 0;
        int[] remainingTime = Arrays.copyOf(burst, n);
        boolean completed[]=new boolean[n];
        for(int i=0;i<n;i++)
        {
            completed[i]=false;
        }
        
        int[] finishTime = new int[n];
        
        int[] waitingTime = new int[n];
        int[] turnaroundTime = new int[n];
    
        System.out.println("Gantt Chart for Round Robin:");
        while (true)
        {
            boolean done = true;
            for(int i=0;i<n;i++)
            {
                if (!completed[i] && arrival[i] <= currentTime)
                {
                    done=false;
                    if(remainingTime[i]<=quantum)
                    {
                        currentTime+=remainingTime[i];
                        completed[i]=true;
                        remainingTime[i]=0;
                        finishTime[i]=currentTime;
                        System.out.print("| P" + i + " ");
                        turnaroundTime[i]=currentTime-arrival[i];
                        waitingTime[i]=turnaroundTime[i]-burst[i];
                    }
                    else
                    {
                        currentTime+=quantum;
                        System.out.print("| P" + i + " ");
                        remainingTime[i]-=quantum;
                    }
                } 
            }
    
            if(done)
            {
                break;
            }
        }
        System.out.println("|");

        double avgWaitingTime=0;
        double avgTurnaroundTime=0;
        
        for(int i=0;i<n;i++)
        {
            avgWaitingTime+=waitingTime[i];
            avgTurnaroundTime+=turnaroundTime[i];
        }
        avgWaitingTime/=n;
        avgTurnaroundTime/=n;
        
        System.out.println("\n");
        System.out.println("Process\tWaiting Time\tTurnaround Time");

        for (int i = 0; i < n; i++)
        {
            System.out.println("P" + i + "\t" + waitingTime[i] + "\t\t" + turnaroundTime[i]);
        }

        System.out.println("\nAverage Waiting Time: " + avgWaitingTime);
        System.out.println("Average Turnaround Time: " + avgTurnaroundTime);
    }
    

    public static void print_process(int arr[], int arrival[], int n) {
        System.out.println("The burst times associated with the processes are : ");
        for (int i = 0; i < n; i++) {
            System.out.print("For process P" + i + ", the burst time is : " + arr[i] + " and the arrival time is : " + arrival[i]);
            System.out.println();
        }
    }

    public static void main(String args[])
    {
        int n, bt, ar, quantum,pr;
        Scanner sc = new Scanner(System.in);
        System.out.print("Enter the number of processes: ");
        n = sc.nextInt();
        int burst[] = new int[n]; // array for storing the burst times for the processes.
        int arrival[] = new int[n]; // array for storing the arrival times
        char process[] = new char[n];
        int priority[]=new int[n];
        for (int i = 0; i < n; i++) {
            System.out.print("Enter the burst time for process " + i + " : ");
            bt = sc.nextInt();
            burst[i] = bt;
            System.out.print("Enter the arrival times for the process " + i + " : ");
            ar = sc.nextInt();
            System.out.print("Enter the priority of the process: " + i + " : ");
            pr=sc.nextInt();
            priority[i]=pr;
            arrival[i] = ar;
            process[i] = (char) ('0' + i); // Assign process names A, B, C, ...
        }
        print_process(burst, arrival, n);

        CPU_Scheduling scheduler = new CPU_Scheduling();

        System.out.print("Enter the time quantum for Round Robin: ");
        quantum = sc.nextInt();
        
        scheduler.preemptive_priority(process, burst, arrival, priority,n);
        scheduler.non_preemptive_sjf(process, burst, arrival, n);
        scheduler.round_robin(process, burst, arrival, n, quantum);

        sc.close();
    }
}
/*public void round_robin(char[] process, int[] burst, int[] arrival, int n, int quantum) {
        int currentTime = 0;
        int[] remainingTime = Arrays.copyOf(burst, n);
        Queue queue = new Queue(n);
        boolean completed[]=new boolean[n];
        for(int i=0;i<n;i++)
        {
            completed[i]=false;
        }
        
        int[] finishTime = new int[n];
    
        queue.enqueue(0); 
    
        System.out.println("Gantt Chart for Round Robin:");
        while (!queue.isEmpty())
        {
            int currentProcess = queue.dequeue();  //currentprocess=0
            if (remainingTime[currentProcess] <= quantum)
            {
                currentTime += remainingTime[currentProcess];
                remainingTime[currentProcess] = 0;
                finishTime[currentProcess] = currentTime;
                completed[currentProcess]=true;
                System.out.print("| P" + currentProcess + " ");
            } 
            else
            {
                completed[currentProcess]=false;
                currentTime += quantum;
                remainingTime[currentProcess] -= quantum;
                System.out.print("| P" + currentProcess + " ");
            }
    
            boolean added = false; // Flag to check if any process was added
    
            for (int i = 0; i < n; i++) {
                if (!queue.contains(i) && arrival[i] <= currentTime && remainingTime[i] > 0)
                {
                    if(burst[i+1]<=remainingTime[i])
                    {
                    queue.enqueue(i);
                    added = true; // A process was added to the queue
                    }
                }
            }
    
            if (!added && !queue.isEmpty()) {
                // No process was added, but the queue is not empty,
                // so there is idle time.
                System.out.print("|   ");
            }
        }
        System.out.println("|");
*/
/*public void preemptive_sjf(char[] process, int[] burst, int[] arrival, int n) {
        int currentTime = 0;
        boolean[] completed = new boolean[n];
        for(int i=0;i<n;i++) //initialising the completed array - denoting that all the processes are yet to be completed
        {
            completed[i]=false;
        }
        int[] remainingTime = Arrays.copyOf(burst, n);
        int[] finishTime = new int[n];

        System.out.println("Gantt Chart for Preemptive SJF:");
        while (true) //every while loop iteration is = the sum of total burst times of all the processes.
        {
            int minBurst = Integer.MAX_VALUE;
            int shortestJob = -1;

            boolean allCompleted = true;

            for (int i = 0; i < n; i++)
            {
                //checking if the i process is not completed and arrival time <= currenttime and remainingtime is less than the burst
                if (completed[i]==false && (arrival[i] <= currentTime) && remainingTime[i] < minBurst)
                {
                    allCompleted = false;
                    minBurst = remainingTime[i];
                    shortestJob = i;
                }
            }

            if (allCompleted) //Terminating condition of the while loop
                break;

            currentTime++;
            if (shortestJob != -1)
            {
                remainingTime[shortestJob]--;
                System.out.print("| P" + shortestJob + " ");
                if (remainingTime[shortestJob] == 0)  //if the process has been completed...
                {
                    completed[shortestJob] = true;  //the shortest process or the current process is completed
                    finishTime[shortestJob] = currentTime;  //store the finishtime
                }
            } 
            else 
            {
                System.out.print("|   "); // Idle time (cpu is idle)
            }
        } //end of while loop

        System.out.println("|");

        // Calculation of average waiting time and turn around time.
        int[] waitingTime = new int[n];
        int[] turnaroundTime = new int[n];

        double totalWaitingTime = 0;
        double totalTurnaroundTime = 0;

        for (int i = 0; i < n; i++)
        {
            turnaroundTime[i] = finishTime[i] - arrival[i];
            waitingTime[i] = turnaroundTime[i] - burst[i];
            totalWaitingTime += waitingTime[i];
            totalTurnaroundTime += turnaroundTime[i];

            System.out.println("Process P" + i + " - Waiting Time: " + waitingTime[i] + ", Turnaround Time: " + turnaroundTime[i]);
        }

        // Calculate and print average waiting time and average turnaround time
        double avgWaitingTime = totalWaitingTime / n;
        double avgTurnaroundTime = totalTurnaroundTime / n;

        System.out.println("Average Waiting Time: " + avgWaitingTime);
        System.out.println("Average Turnaround Time: " + avgTurnaroundTime);
    }
*/