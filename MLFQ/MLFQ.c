#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
//------------------
// GLOBAL VARIABLES
//------------------
int createTask = 0;
int doneTask = 0;
int timeAllotment = 0;
int cpu_num = 0;
bool Terminate = false;

struct timespec diff(struct timespec start, struct timespec end);
void msleep(unsigned int microseconds);

//------------------
// TASK
//------------------

typedef struct Task
{ // bunch of infomation
    //- input
    char *name;
    int type;
    int length;
    int odds_IO;

    // priority management
    int priority;
    int timeSpend;

    //- analysis
    struct timespec arriveTime;
    struct timespec completionTime;
    struct timespec response;

    struct Task *next;
} task_unit;

//------------------
// QUEUE
//------------------

typedef struct Queue
{
    task_unit *head;
    task_unit *tail;
    int id;
} queue;

queue *A;
queue *B;
queue *C;
queue *D;
queue *Done;

queue *queueCreate(int id)
{
    queue *q;
    q = (queue *)malloc(sizeof(queue));
    if (q != NULL)
    {
        q->head = NULL;
        q->tail = NULL;
        q->id = id;
    }

    else
    {
        printf("it is null pointer, failure to create queue.");
    }
    return q;
}
bool emptyQueue(queue *q)
{
    if (q == NULL)
    {
        printf("q is null!");
    }
    return (q->head == NULL);
}

void enqueue(queue *q, task_unit *t)
{
    t->next = NULL;
    if (q == NULL)
    {
        printf("q is null!!!!");
        if (t == NULL)
        {
            printf("t is null!!!!");
        }
    }

    //  two case
    //  if head is null
    if (emptyQueue(q))
    {
        q->head = t;
        q->tail = t;
    }
    // else if head is not null
    else
    {
        q->tail->next = t;
        q->tail = t;
    }
}

task_unit *dequeue(queue *q)
{

    task_unit *t = NULL;
    if (q->head != NULL)
    {
        t = q->head;
        if (t->next == NULL)
        {
            q->head = NULL;
        }
        else
        {
            q->head = t->next;
            t->next = NULL;
        }

        // Check if the queue became empty after dequeueing
        if (q->head == NULL)
        {
            q->tail = NULL; // Update tail pointer to NULL
        }
        t->next = NULL;
    }
    else
    {
        return NULL;
    }

    return t;
}

//------------------
// READING THREAD
//------------------

task_unit *readTask(char *line)
{
    task_unit *t = (task_unit *)malloc(sizeof(task_unit));
    if (t == NULL)
    {
        // Handle memory allocation failure
        return NULL;
    }

    char *token = strchr(line, '\n');
    if (token != NULL)
    {
        *token = ' ';
    }

    token = strtok(line, " ");
    if (token == NULL)
    {
        // Handle empty line or other error condition
        free(t); // Free allocated memory before returning
        return NULL;
    }

    // Allocate memory for t->name and copy token into it
    t->name = (char *)malloc(strlen(token) + 1);
    if (t->name == NULL)
    {
        // Handle memory allocation failure
        free(t); // Free allocated memory before returning
        return NULL;
    }
    strcpy(t->name, token);

    // Parse and assign other fields
    token = strtok(NULL, " ");
    if (token == NULL)
    {
        // Handle error condition (insufficient tokens)
        free(t->name); // Free allocated memory before returning
        free(t);
        return NULL;
    }
    t->type = atoi(token);

    token = strtok(NULL, " ");
    if (token == NULL)
    {
        // Handle error condition (insufficient tokens)
        free(t->name); // Free allocated memory before returning
        free(t);
        return NULL;
    }
    t->length = atoi(token);

    token = strtok(NULL, " ");
    if (token == NULL)
    {
        // Handle error condition (insufficient tokens)
        free(t->name); // Free allocated memory before returning
        free(t);
        return NULL;
    }
    t->odds_IO = atoi(token);

    t->priority = 0;
    t->timeSpend = 0;
    t->next = NULL;
    createTask++;

    return t;
}

// compare "DELAY", if it is delay, bool return false, is task, return true
bool isTask(char *line)
{
    char isDelay[] = {line[0], line[1], line[2], line[3], line[4], '\0'};
    if (strcmp(isDelay, "DELAY") == 0)
    {
        return false;
    }
    return true;
}

#define NANOS_PER_USEC 1000
#define USEC_PER_SEC 1000000
void msleep(unsigned int usecs)
{
    long seconds = usecs / USEC_PER_SEC;
    long nanos = (usecs % USEC_PER_SEC) * NANOS_PER_USEC;
    struct timespec t = {.tv_sec = seconds, .tv_nsec = nanos};
    int ret;
    do
    {
        ret = nanosleep(&t, &t);
        // need to loop, `nanosleep` might return before sleeping
        // for the complete time (see `man nanosleep` for details)
    } while (ret == -1 && (t.tv_sec || t.tv_nsec));
}

//------------------
// MLFQ
//------------------
pthread_mutex_t Alock;
pthread_mutex_t Block;
pthread_mutex_t Clock;
pthread_mutex_t Dlock;
pthread_mutex_t Donelock;
pthread_mutex_t cpuLock;
pthread_mutex_t termLock;

void enMLFQ(task_unit *t)
{

    // control when to access the first queue.
    if (t->priority == 0)
    {
        pthread_mutex_lock(&Alock);
       
	enqueue(A, t);

        pthread_mutex_unlock(&Alock);
    }

    else if (t->priority == 1)
    {
        pthread_mutex_lock(&Block);
        
	enqueue(B, t);
        
	pthread_mutex_unlock(&Block);
    }
    else if (t->priority == 2)
    {
        pthread_mutex_lock(&Clock);
        
	enqueue(C, t);
        
	pthread_mutex_unlock(&Clock);
    }
    else if (t->priority == 3)
    {
        pthread_mutex_lock(&Dlock);
        
	enqueue(D, t);
        
	pthread_mutex_unlock(&Dlock);
    }
}

task_unit *deMLFQ(void)
{

    task_unit *t = NULL;

    if (!emptyQueue(A))
    {
        pthread_mutex_lock(&Alock);

        t = dequeue(A);

        pthread_mutex_unlock(&Alock);
    }
    else if (!emptyQueue(B))
    {
        pthread_mutex_lock(&Block);

        t = dequeue(B);

        pthread_mutex_unlock(&Block);
    }
    else if (!emptyQueue(C))
    {
        pthread_mutex_lock(&Clock);

        t = dequeue(C);

        pthread_mutex_unlock(&Clock);
    }
    else if (!emptyQueue(D))
    {
        pthread_mutex_lock(&Dlock);

        t = dequeue(D);

        pthread_mutex_unlock(&Dlock);
    }

    if (t == NULL)
    {
        return NULL;
    }
    return t;
}

void resetMLFQ(void)
{
    task_unit *t;
    // lock whole mlfq except the first
    pthread_mutex_lock(&Block);

    while (!emptyQueue(B))
    {
        t = dequeue(B);
        t->priority = 0;
        t->timeSpend = 0;

        enMLFQ(t);
    }
    pthread_mutex_unlock(&Block);
    
    pthread_mutex_lock(&Clock);
    while (!emptyQueue(C))
    {
        t = dequeue(C);
        t->priority = 0;
        t->timeSpend = 0;

        enMLFQ(t);
    }
    pthread_mutex_unlock(&Clock);
    
    pthread_mutex_lock(&Dlock);
    while (!emptyQueue(D))
    {
        t = dequeue(D);
        t->priority = 0;
        t->timeSpend = 0;

        enMLFQ(t);
    }
    pthread_mutex_unlock(&Dlock);

}

void *resetThread(__attribute__((unused)) void *s)
{
    while (!Terminate)
    {
        msleep(timeAllotment);
        resetMLFQ();
    }
    pthread_exit(NULL);
}

void enterDone(task_unit *t)
{

    pthread_mutex_lock(&Donelock);

    enqueue(Done, t);

    doneTask++;
    pthread_mutex_unlock(&Donelock);
}

//------------------
// CPU
//------------------

pthread_cond_t canRun = PTHREAD_COND_INITIALIZER;
const int quantum = 50;

void *cpuThread(__attribute__((unused)) void *s)
{
    task_unit *t = NULL;
    int random;

    while (!Terminate)
    {

        // while running, it take the task from mlfq, then check for io chance, knows the time for exe if io true,
        // exe is sleep with ms, then it can enter the queue with priority -1
        //  if the task is finished, eg, length =0, it enter done q
        pthread_cond_wait(&canRun, &cpuLock);

        t = deMLFQ();

        random = rand() % 100;

        if (t != NULL)
        {

            // get response time for every first-encounter task
            if (t->response.tv_nsec == 0)
            {
                clock_gettime(CLOCK_REALTIME, &t->response);
            }

            // If the number you generate is less than or equal to the “odds_of_IO” for a task,
            // then the task will do I/O.
            // If the number you generate is greater than the “odds_of_IO” for a task,
            // then the task will not do I/O.
            if (random > t->odds_IO)
            {
                if (t->length <= quantum)
                {

                    msleep(t->length);
                    t->length = 0;

                    clock_gettime(CLOCK_REALTIME, &t->completionTime);

                    enterDone(t);
                }
                else
                {
                    msleep(quantum);
                    t->length -= quantum;
                    t->timeSpend += quantum;

                    if (t->priority < 3 && t->timeSpend > 200)
                    {
                        t->priority++;
                        t->timeSpend = 0;
                    }

                    enMLFQ(t);
                }
            }
            else
            {
                if (t->length < quantum)
                {
                    random = (rand() % t->length) + 1;
                }
                else
                {
                    random = (rand() % quantum) + 1;
                }
                msleep(random);

                t->length -= random;
                t->timeSpend += random;

                if (t->priority < 3 && t->timeSpend > 200)
                {
                    t->priority++;
                    t->timeSpend = 0;
                }

                if (t->length > 0)
                {
                    enMLFQ(t);
                }
                else
                {
                    clock_gettime(CLOCK_REALTIME, &t->completionTime);
                    enterDone(t);
                }
            }
        }
    }
    pthread_exit(NULL);
}

//------------------
// TIME
//------------------

// Source by Guy Rutenberg PROVIDED IN UMLEARN
struct timespec diff(struct timespec start, struct timespec end)
{
    struct timespec temp;
    if ((end.tv_nsec - start.tv_nsec) < 0)
    {
        temp.tv_sec = end.tv_sec - start.tv_sec - 1;
        temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
    }
    else
    {
        temp.tv_sec = end.tv_sec - start.tv_sec;
        temp.tv_nsec = end.tv_nsec - start.tv_nsec;
    }
    return temp;
}

//------------------
// MAIN
//------------------

int main(int args, char *argv[])
{
    // 1. reading thread
    FILE *file;
    char line[100];
    task_unit *t;
    pthread_mutex_init(&Alock, NULL);
    pthread_mutex_init(&Block, NULL);
    pthread_mutex_init(&Clock, NULL);
    pthread_mutex_init(&Dlock, NULL);
    pthread_mutex_init(&Donelock, NULL);
    pthread_mutex_init(&cpuLock, NULL);
    pthread_mutex_init(&termLock, NULL);

    pthread_t *cores;
    pthread_t reset;
    int resetFlag = 0;

    if (args != 4)
    {
        printf("The command does not satisify the input format\n");
    }
    else
    {
        printf("Receiving Command...\n");
        cpu_num = atoi(argv[1]);
        timeAllotment = atoi(argv[2]);

        printf("This MLFQ stimulation will have %d CPU and reset priority after %d us.\n", cpu_num, timeAllotment);

        timeAllotment *= 1000;

        A = queueCreate(0);
        B = queueCreate(1);
        C = queueCreate(2);
        D = queueCreate(3);
        Done = queueCreate(4);

        cores = malloc(sizeof(pthread_t) * cpu_num);

        for (int i = 0; i < cpu_num; i++)
        {
            pthread_create(&cores[i], NULL, cpuThread, NULL);
        }

        file = fopen(argv[3], "r");
        if (file == NULL)
        {
            printf("it is an empty file.\n");
        }
        else
        {

            while (fgets(line, 100, file) != NULL)
            {
                if (isTask(line))
                {

                    t = readTask(line);

                    if (t == NULL)
                    {
                        exit(0);
                    }
                    clock_gettime(CLOCK_REALTIME, &t->arriveTime);
                    enMLFQ(t);
                               
                }
                else
                {
                    // Some tasks are initially loaded from a file and are put in the scheduler’s ready queue before the start of the simulation (
                    //   all the tasks that appear before the first DELAY line).
                    if (resetFlag == 0)
                    {

                        pthread_create(&reset, NULL, resetThread, NULL);
                        pthread_cond_signal(&canRun);
                        resetFlag = 1;
                    }

                    msleep(atoi(&line[6]));
                }
            }
            fclose(file);
        }
    }

    //2. Signal/Dispatch
    while (createTask > doneTask)
    {

        pthread_cond_signal(&canRun);
    }


    pthread_mutex_lock(&termLock);
    Terminate = true;
    pthread_mutex_unlock(&termLock);

    // 3. make analysis
    long turnaround[4] = {0, 0, 0, 0};
    long response[4] = {0, 0, 0, 0};
    int amount_type[4] = {0, 0, 0, 0};
    struct timespec elapseTime;
    struct timespec responseTime;

    while (!emptyQueue(Done))
    {
        pthread_mutex_lock(&Donelock);
        t = dequeue(Done);
        pthread_mutex_unlock(&Donelock);

        if (t != NULL)
        {
            elapseTime = diff(t->arriveTime, t->completionTime);
            responseTime = diff(t->arriveTime, t->response);

            amount_type[t->type]++;
            turnaround[t->type] += elapseTime.tv_nsec / 1000;
            response[t->type] += responseTime.tv_nsec / 1000;
        }

        free(t->name);
        free(t);
    }

    long avgTurnaround[4] = {0, 0, 0, 0};
    long avgResponse[4] = {0, 0, 0, 0};
    for (int i = 0; i < 4; i++)
    {
        printf("total amount for type %d :%d, total turnaround: %lu\n", i, amount_type[i], turnaround[i]);
        avgTurnaround[i] = turnaround[i] / amount_type[i];
        printf("total amount for type %d :%d, total response: %lu\n", i, amount_type[i], response[i]);
        avgResponse[i] = response[i] / amount_type[i];
    }

    printf("\nAverage Turnaround time per type:\n");
    for (int i = 0; i < 4; i++)
    {
        printf("Type %d: %lu usec\n", i, avgTurnaround[i]);
    }

    printf("\nAverage Response time per type:\n");
    for (int i = 0; i < 4; i++)
    {
        printf("Type %d: %lu usec\n", i, avgResponse[i]);
    }

    //4. Free/Destroy
    pthread_mutex_destroy(&Alock);
    pthread_mutex_destroy(&Block);
    pthread_mutex_destroy(&Clock);
    pthread_mutex_destroy(&Dlock);
    pthread_mutex_destroy(&Donelock);
    pthread_mutex_destroy(&cpuLock);
    pthread_mutex_destroy(&termLock);

    printf("Freeing Program...\n");
    sleep(1);
    free(A);
    free(B);
    free(C);
    free(D);
    free(Done);
    free(cores);
    printf("Successful\n");
    printf("Program Halted\n");
    return 0;
}
