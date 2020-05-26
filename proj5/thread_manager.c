#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>


//thread mutex lock for access to the log index
pthread_mutex_t tlock1 = PTHREAD_MUTEX_INITIALIZER;
//thread mutex lock for critical sections of allocating THREADDATA
pthread_mutex_t tlock2 = PTHREAD_MUTEX_INITIALIZER;



pthread_t tid1, tid2;
struct THREADDATA_STRUCT
{
    pthread_t creator;
};

typedef struct THREADDATA_STRUCT THREADDATA;

typedef struct list_node{
    char str[100];
    struct list_node* next;
} node_t;

THREADDATA* p = NULL;
char line[100];
char time_str[100];
node_t* list = NULL;
//variable for indexing of messages by the logging function
int logindex=0;
int *logip = &logindex;
void* thread_runner(void*);
char* get_time(void);
void insert(char* str);



//A flag to indicate if the reading of input is complete,
//so the other thread knows when to stop
bool is_reading_complete = false;


/*********************************************************
 // function main  -------------------------------------------------
 *********************************************************/
int main()
{
    
    printf("create first thread\n");
    pthread_create(&tid1,NULL,thread_runner,NULL);
    
    printf("create second thread\n");
    pthread_create(&tid2,NULL,thread_runner,NULL);
    
    printf("wait for first thread to exit\n");
    pthread_join(tid1,NULL);
    printf("first thread exited\n");
    
    printf("wait for second thread to exit\n");
    pthread_join(tid2,NULL);
    printf("second thread exited\n");
    
    node_t* n = list;
    while(list != NULL) {
        list = list->next;
        free(n);
        n = list;
    }
    pthread_mutex_destroy(&tlock1);
    pthread_mutex_destroy(&tlock2);
    exit(0);
    
}//end main


/**********************************************************************
 // function thread_runner runs inside each thread --------------------------------------------------
 **********************************************************************/
void* thread_runner(void* x)
{
    pthread_t me;
    
    me = pthread_self();
    printf("This is thread %ld (p=%p)\n", me, p);
    
    pthread_mutex_lock(&tlock2); // critical section starts
    if (p == NULL) {
        p = (THREADDATA*) malloc(sizeof(THREADDATA));
        p->creator = me;
    }
    pthread_mutex_unlock(&tlock2);  // critical section ends
    
    if (p != NULL && p->creator == me)
        printf("This is thread %ld and I created the THREADDATA %p\n",me,p);
    else
        printf("This is thread %ld and I can access the THREADDATA %p\n",me,p);
    while(is_reading_complete == false){
        
        if(p->creator == me){
            pthread_mutex_lock(&tlock1);
            get_time();
            if(logindex%2 == 0){
                fgets(line, 100, stdin);
                if(strlen(line) == 1){
                    is_reading_complete = true;
                    pthread_mutex_unlock(&tlock1);
                    break;
                }
                insert(line);
                printf("Logindex %d, thread 1, PID %ld, %s: Head of linked list contains %s",logindex, me,time_str,line);
                logindex++;
            }
            pthread_mutex_unlock(&tlock1);
            
        }
        else {
            pthread_mutex_lock(&tlock1);
            get_time();
            if(logindex%2 == 1){
                fputs(line,stdout);
                printf("Logindex %d, thread 2, PID %ld, %s: Head of linked list contains %s",logindex,me,time_str,line);
                logindex++;
            }
            pthread_mutex_unlock(&tlock1);
        }
        
    }
    
    pthread_mutex_lock(&tlock2);
    if (p != NULL && p->creator == me)
        printf("This is thread %ld and I didnot touch THREADDATA p = %p\n",me, p);
    else{
        if (p != NULL) {
            free(p);
            p = NULL;
            printf("This is thread %ld and I deleted the THREADDATA p = %p\n",me, p);
        }
    }
    pthread_mutex_unlock(&tlock2);
    pthread_exit(NULL);
    return NULL;
    
}

char* get_time(){
    // variables to store date and time components
    int hours, minutes, seconds, day, month, year;
    
    // time_t is arithmetic time type
    time_t now;
    
    // Obtain current time
    // time() returns the current time of the system as a time_t value
    time(&now);
    
    
    // gets the current time
    struct tm *local = localtime(&now);
    
    hours = local->tm_hour;          // get hours since midnight (0-23)
    minutes = local->tm_min;         // get minutes passed after the hour (0-59)
    seconds = local->tm_sec;         // get seconds passed after minute (0-59)
    
    day = local->tm_mday;            // get day of month (1 to 31)
    month = local->tm_mon + 1;       // get month of year (0 to 11)
    year = local->tm_year + 1900;    // get year since 1900
    
    
    // prints current date
    sprintf(time_str,"%02d/%02d/%d ", day, month, year);
    
    char tmp[100];
    if (hours < 12)    // before midday
        sprintf(tmp,"%02d:%02d:%02d am", hours, minutes, seconds);
    
    else    // after midday
        sprintf(tmp,"%02d:%02d:%02d pm", hours - 12, minutes, seconds);
    strcat(time_str,tmp);
    
    return time_str;
}

void insert(char* str){
    node_t* node = (node_t*)malloc(sizeof(node_t));
    strcpy(node->str,str);
    if(list == NULL){
        list = node;
        list->next=NULL;
    }else{
        node->next = list;
        list = node;
    }
}
