#include <iostream>
using namespace std;
class Queue
{
    private:
    int front=-1;
    int rear=-1;
    int size;
    int* queue;
    
    public:
    Queue(int size)
    {
        this->size=size;
        this->queue=new int[size];
    }
    ~Queue()
    {
        delete[] queue;
    }
    bool isFull() {
            return rear-front==size-1;
        }

    bool isEmpty()
    {
        return front == -1;
    }

    void enqueue(int item)
    {
        if (isFull())
        {
            return;
        }
        if (front == -1)
        {
            front++;
        }
        rear++;
        queue[rear] = item;
    }

    int dequeue()
    {
        int item;
        if (isEmpty())
        {
            return -1;
        }
        else
        {
            item = queue[front];
        }
        if(front >= rear)
        {
            front = rear = -1;
        }
        else
        {
            front++;
        }
        return item;
    }
    bool find(int item)
    {
        for(int i=front;i<=rear;i++)
        {
            if(item==queue[i])
            {
                return true;
            }
        }
        return false;
    }
    void print()
    {
        for(int i=front;i<=rear;i++)
        {
            cout<<queue[i]<<" ";
        }
    }
};
void fifo(int ref_string[],int nf,int nr)
{
    int page_faults=0;
    int hit=0;
    int miss=0;
    Queue frames(nf);
    for(int i=0;i<nr;i++)
    {
        //check if the given page is present in any of the frames or not.
        if(frames.find(ref_string[i]))
        {
            cout<<"HIT"<<endl;
            hit++;
        }
        else
        {
            if(frames.isFull())
            {
                //fifo replacement algorithm
                cout<<"Full - replacement algorithm occurs! "<<endl;
                frames.dequeue();
                frames.enqueue(ref_string[i]);
                page_faults++;
                miss++;
            }
            else
            {
            frames.enqueue(ref_string[i]);
            cout<<"Enqueued"<<endl;
            page_faults++;
            miss++;
            }
        }
        cout<<"The frames are : "<<endl;
        frames.print();
        cout<<endl;
    }
    if (nr > 0)
    {
    float hit_ratio = static_cast<float>(hit) / nr;
    cout<<"The number of hits is: "<<hit;
    cout<<"Number of page faults = "<<page_faults;
    cout << "The hit ratio is: (number of hits/number of attempts):  " << hit_ratio << endl;
    }
    else
    {
    cout << "The hit ratio is undefined because there are no page references." << endl;
    }
}

//least recently used algorithm
// we can use the lru algorithm by returning the minimum index of each of the elements in the queue and whose index is minimum, that element is dequed.
void printframe(int frames[],int nf)
{
    cout<<"Updated frames array: "<<endl;
    for(int i=0;i<nf;i++)
    {
        cout<<frames[i]<<" ";
    }
    cout<<endl;
}
bool find(int arr[],int nf,int item)
{
    for(int i=0;i<nf;i++)
    {
        if(item==arr[i])
        {
            return true;
        }
    }
    return false;
}
void calculateAges(int age[], int frames[], int nf, int currSize, int ref_string[], int index) {
    for (int i = currSize - 1; i >= 0; i--) {
        int page = frames[i];

        // Find the most recent reference to the page.
        int recentIndex = -1;
        for (int j = index - 1; j >= 0; j--) {
            if (ref_string[j] == page) {
                recentIndex = j;
                break;
            }
        }

        // Calculate the age of the page as the number of references since the most recent one.
        age[i]=recentIndex;
    }
}
int findIndexOfMinAge(int age[], int nf) {
    int minAge = age[0];
    int minIndex = 0;
    for (int i = 1; i < nf; i++) {
        if (age[i] < minAge) {
            minAge = age[i];
            minIndex = i;
        }
    }
    return minIndex;
}

void lru(int ref_string[],int nf,int nr)
{
    int hits=0;
    int page_faults=0;
    int miss=0;
    int curr_index_f=0;
    int frames[nf]={0};
    int age[nf];
    for(int i=0;i<nr;i++)
    {
        if(find(frames,nf,ref_string[i]))
        {
            cout<<"HIT"<<endl;
            hits++;
        }
        else
        {
            if(curr_index_f==nf)
            {
                cout<<"Full ! - LRU Algorithm occurs !"<<endl;
                calculateAges(age,frames,nf,curr_index_f,ref_string,i);
                frames[findIndexOfMinAge(age,nf)]=ref_string[i];
                page_faults++;
                miss++;
            }
            else
            {
                frames[i]=ref_string[i];
                miss++;
                page_faults++;
                curr_index_f++;
            }
        }
        printframe(frames,nf);
    }
    if (nr > 0)
    {
    float hit_ratio = static_cast<float>(hits) / nr;
    cout<<"The number of hits is: "<<hits<<endl;
    cout<<"Number of page faults = "<<page_faults<<endl;
    cout << "The hit ratio is: (number of hits/number of attempts):  " << hit_ratio << endl;
    }
    else
    {
    cout << "The hit ratio is undefined because there are no page references." << endl;
    }
}
void calculateAgesforOptimal(int age[], int frames[],int currSize, int ref_string[], int index,int nr) {
    for (int i = currSize - 1; i >= 0; i--) {
        int page = frames[i];

        // Find the most recent reference to the page.
        int recentIndex = -1;
        for (int j = index; j<nr; j++) {
            if (ref_string[j] == page) {
                recentIndex = j;
                break;
            }
        }

        // Calculate the age of the page as the number of references since the most recent one.
        if(recentIndex==-1)
        {
            age[i]=100; //the page number is not needed in the future, so give max. age.
        }
        else
        {
        age[i]=recentIndex;
        }
    }
}
int findIndexOfMaxAge(int age[], int nf) {
    int maxAge = age[0];
    int maxIndex = 0;
    for (int i = 1; i < nf; i++) {
        if (age[i] > maxAge) {
            maxAge = age[i];
            maxIndex = i;
        }
    }
    return maxIndex;
}
void optimal(int ref_string[],int nf,int nr)
{
    int hits=0;
    int page_faults=0;
    int frames[nf]={0};
    int age[nf]={0};
    int curr_size=0;
    for(int i=0;i<nr;i++)
    {
        if(find(frames,nf,ref_string[i]))
        {
            cout<<"HIT"<<endl;
            hits++;
        }
        else
        {
            if(curr_size==nf)
            {
                //optimal algorithm starts
                cout<<"Full - Optimal Algorithm occurs!"<<endl;
                calculateAgesforOptimal(age,frames,curr_size,ref_string,i,nr);
                frames[findIndexOfMaxAge(age,nf)]=ref_string[i];
                page_faults++;
            }
            else
            {
                frames[curr_size]=ref_string[i];
                page_faults++;
                curr_size++;
            }
        }
        printframe(frames,nf);
    }
    if (nr > 0)
    {
    float hit_ratio = static_cast<float>(hits) / nr;
    cout<<"The number of hits is: "<<hits<<endl;
    cout<<"Number of page faults = "<<page_faults<<endl;
    cout << "The hit ratio is: (number of hits/number of attempts):  " << hit_ratio << endl;
    }
    else
    {
    cout << "The hit ratio is undefined because there are no page references." << endl;
    }
}
int main()
{
    int refrence_string[100];
    int nf,nr;
    cout<<"Enter the number of references: ";
    cin>>nr;
    cout<<"Enter the reference string of requested processes: "<<endl;
    for(int i=0;i<nr;i++)
    {
        cout<<"Enter reference "<<i<<"  :";
        cin>>refrence_string[i];
    }
    cout<<"Enter the number of frames: ";
    cin>>nf;
    cout<<"Printing the reference string: "<<endl;
    for(int i=0;i<nr;i++)
    {
        cout<<"Reference "<<i<<"  :"<< refrence_string[i]<<endl;
    }
    // fifo(refrence_string,nf,nr);
    // cout<<"Using least recently used algorithm: "<<endl<<endl;
    // lru(refrence_string,nf,nr);
    cout<<"Using optimal algorithm: "<<endl<<endl;
    optimal(refrence_string,nf,nr);
}