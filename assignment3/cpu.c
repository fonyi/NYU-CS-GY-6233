#include "oslabs.h"

//checks for a null pcb
int check_null(struct PCB pcb){
    //checks for null process
	if(pcb.process_id == 0 && pcb.arrival_timestamp == 0 && pcb.total_bursttime == 0 && pcb.execution_starttime == 0 && pcb.execution_endtime == 0 && pcb.remaining_bursttime == 0 && pcb.process_priority == 0){
        return 1;
    }
    else{
        return 0;
    }
}


//sets pcb
void set_pcb(struct PCB *to_set, int pid, int ats, int tbt, int est, int eet, int rbt, int pp){
    to_set->process_id = pid;
    to_set->arrival_timestamp = ats;
    to_set->total_bursttime = tbt;
    to_set->execution_starttime = est;
    to_set->execution_endtime = eet;
    to_set->remaining_bursttime = rbt;
    to_set->process_priority = pp;
}

//queues pcb
int enqueue_pcb(struct PCB ready_queue[], int* queue_cnt, struct PCB* to_enqueue){
    // Queue is Full - return 0 as error code
    if((*queue_cnt) >= QUEUEMAX)
        return 0;

    ready_queue[(*queue_cnt)] = (*to_enqueue);
    (*queue_cnt)++;

    return 1;
}

//handle arrival pp
struct PCB handle_process_arrival_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp)
{
   //check for process
    if(check_null(current_process)){
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    }

    if(new_process.process_priority >= current_process.process_priority){
        if((*queue_cnt) < QUEUEMAX){
            new_process.execution_starttime = 0;
            new_process.execution_endtime = 0;
            new_process.remaining_bursttime = new_process.total_bursttime;

            ready_queue[*queue_cnt] = new_process;
            (*queue_cnt)++;
        }

        
        return current_process;
    }

    if((*queue_cnt) < QUEUEMAX){
        current_process.execution_endtime = 0;
        current_process.remaining_bursttime = current_process.total_bursttime - 1;

        ready_queue[*queue_cnt] = current_process;
        (*queue_cnt)++;
    }


    new_process.execution_starttime = timestamp;
    new_process.execution_endtime = timestamp + new_process.total_bursttime;


    return new_process;

}

// handle completion pp
struct PCB handle_process_completion_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp){

    //clear queue
    if((*queue_cnt) == 0){
        struct PCB null_pcb;
        set_pcb(&null_pcb, 0, 0, 0, 0, 0, 0, 0);

        return null_pcb;
    }

    int high_index = 0;

    for (int i = 1; i < (*queue_cnt); i++){
        if (ready_queue[high_index].process_priority > ready_queue[i].process_priority)
            high_index = i;
    }

    struct PCB high_priority = ready_queue[high_index];

    //shift
    for (int i = high_index; i < (*queue_cnt); i++){
        ready_queue[i] = ready_queue[i+1];
    }

    *queue_cnt = *queue_cnt -1;

    high_priority.execution_starttime = timestamp;
    high_priority.execution_endtime = timestamp + high_priority.remaining_bursttime;

    return high_priority; 
}

//handle arrival srtp
struct PCB handle_process_arrival_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int time_stamp){
    //check process
    if(check_null(current_process)){
        new_process.execution_starttime = time_stamp;
        new_process.execution_endtime = time_stamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    }

    else if(current_process.remaining_bursttime < new_process.remaining_bursttime){
        ready_queue[(*queue_cnt)] = new_process;
        (*queue_cnt)++;
        new_process.execution_starttime = 0;
        new_process.execution_endtime = 0;
        new_process.remaining_bursttime = new_process.total_bursttime;

        return current_process;
    }
    current_process.execution_starttime = 0;
    current_process.execution_endtime = 0;
    current_process.remaining_bursttime = current_process.total_bursttime;
    ready_queue[(*queue_cnt)] = current_process;
    (*queue_cnt)++;
    new_process.execution_starttime = time_stamp;
    new_process.execution_endtime = time_stamp + new_process.total_bursttime;
    new_process.remaining_bursttime = new_process.total_bursttime;
    
    return new_process;
}


//handle completion srtp
struct PCB handle_process_completion_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp){
    //clear queue
    if((*queue_cnt) == 0){
        struct PCB null_pcb;
        set_pcb(&null_pcb, 0, 0, 0, 0, 0, 0, 0);

        return null_pcb;
    }
    int srbt_index = 0;

    for (int i = 1; i < (*queue_cnt); i++){
        if (ready_queue[srbt_index].remaining_bursttime > ready_queue[i].remaining_bursttime)
            srbt_index = i;
    }

    struct PCB srbt = ready_queue[srbt_index];

    for (int i = srbt_index; i < (*queue_cnt); i++){
        ready_queue[i] = ready_queue[i+1];
    }

    *queue_cnt = *queue_cnt - 1;

    srbt.execution_starttime = timestamp;
    srbt.execution_endtime = timestamp + srbt.remaining_bursttime;

    return srbt;
}

//handle arrival rr
struct PCB handle_process_arrival_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp, int time_quantum){
    if(check_null(current_process)){
         new_process.execution_starttime = timestamp;
         new_process.execution_endtime = timestamp + MIN(new_process.total_bursttime, time_quantum);
         new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    }


    new_process.execution_starttime = 0;
    new_process.execution_endtime = 0;
    new_process.remaining_bursttime = new_process.total_bursttime;
    ready_queue[(*queue_cnt)] = new_process;
    (*queue_cnt)++;
    
    return current_process;
}

//handle completion rr
struct PCB handle_process_completion_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp, int time_quantum){
    //clear queue
    if((*queue_cnt) == 0){
        struct PCB null_pcb;
        set_pcb(&null_pcb, 0, 0, 0, 0, 0, 0, 0);

        return null_pcb;
    }
    int earliest_time_index = 0;

    for (int i = 1; i < (*queue_cnt); i++){
        if (ready_queue[earliest_time_index].arrival_timestamp > ready_queue[i].arrival_timestamp)
            earliest_time_index = i;
    }

    struct PCB earliest_time = ready_queue[earliest_time_index];

    //shift
    for (int i = earliest_time_index; i < (*queue_cnt); i++){
        ready_queue[i] = ready_queue[i+1];
    }

    *queue_cnt = *queue_cnt - 1;
    earliest_time.execution_starttime = timestamp;
    earliest_time.execution_endtime = timestamp + MIN(time_quantum, earliest_time.remaining_bursttime);

    return earliest_time;
}

