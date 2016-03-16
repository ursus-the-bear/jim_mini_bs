#include <stdio.h>
#include <stdlib.h>

/* Kleines BS in c*/
struct _task;

typedef struct _events{
    int zuTaskId;
    struct _task* backUpTask;
    void (*setEvent)();
}Events_T;

Events_T gevent;

typedef struct _task {
    int taskId;
    void (*init) (int taskID);
    void (*poll) (struct _task* self);
    void (*processEvent)(int eventId);
    struct _task* nextTask;
}Task_T;

Task_T * aktTask;

void Task1Func(int id){
    printf("Task nr %i laeuft\n", id);
}

void pollTask(struct _task* self){
    printf("Task id %i called Poll\n", self->taskId);
}

void taskEvent(int eventId){
    
    printf ("in Eventhandler. TaskID: %i\n", eventId);
    
}


Task_T* createTask(Task_T* self ){
    
    static int  id = 1;
    struct _task* newTask;
    
    newTask = (Task_T*) malloc(sizeof(struct _task));
    
    newTask->taskId = id;
    //newTask->init= &initMyTask; // wird nicht benˆtigt da das eig in Create gemacht wird
    newTask->poll= &pollTask;
    newTask->processEvent = &taskEvent;
    
    newTask->nextTask = 0;
    id++;
    
    if(self == 0){
        
        self = newTask;
    }else{
        Task_T* tmpTask;
        tmpTask = self;
        while(tmpTask->nextTask !=0){
            tmpTask = tmpTask->nextTask;
        }
        tmpTask->nextTask = newTask;
    }
    
    return self;
}


void EventMelden(){
    
    if(gevent.zuTaskId != 0){
        gevent.backUpTask = aktTask; /*sichern vom Aktellen Task*/
        
        while(aktTask->taskId != gevent.zuTaskId ){
            aktTask = aktTask->nextTask;
        }
        
        aktTask->poll(aktTask);
        aktTask = gevent.backUpTask; // speicher alten zur¸ck
        gevent.zuTaskId = 0;
    }
}


int main(){
    
    gevent.setEvent = &EventMelden;
    
    aktTask = createTask(aktTask);
    aktTask = createTask(aktTask);
    aktTask = createTask(aktTask);
    aktTask = createTask(aktTask);
    
    
    while(aktTask->nextTask != 0){
        if(aktTask->taskId == 3){
            aktTask->processEvent(1);
        }
        aktTask->poll(aktTask);
        
        // wechseln zum nächsten task
        aktTask = aktTask->nextTask;
    }
    
    
    
    return 0;
}

