#include "oslabs.h"

//empty rcb
struct RCB empty_rcb = {0,0,0,0,0};

//check for null rcb
int check_null(struct RCB rcb){
    if(rcb.request_id == 0 && rcb.arrival_timestamp == 0 && rcb.cylinder == 0 && rcb.address == 0 && rcb.process_id ==0){
        return 1;
    }
    else
        return 0;    
}

//arrival fcfs
struct RCB handle_request_arrival_fcfs(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp){
    if(check_null(current_request)){
		return new_request;
	}
	else
		request_queue[*queue_cnt] = new_request;
		(*queue_cnt) = (*queue_cnt) + 1;
		return current_request;
}
//complete fcfs
struct RCB handle_request_completion_fcfs(struct RCB request_queue[QUEUEMAX],int *queue_cnt){
    if(*queue_cnt == 0){
		return empty_rcb;
    }
	
	int firstArrive = 0;
	
	for (int i = 1; i < *queue_cnt; i++)
	{
		if (request_queue[i].arrival_timestamp < request_queue[firstArrive].arrival_timestamp)
		{
			firstArrive = i;
		}
	}
	struct RCB remove = request_queue[firstArrive];
	for (int i = firstArrive -1; i < *queue_cnt - 1; i++)
	{
		request_queue[i] = request_queue[i+1];
	}
	(*queue_cnt) = (*queue_cnt) - 1;
	return remove;	
}
//arrival sstf
struct RCB handle_request_arrival_sstf(struct RCB request_queue[QUEUEMAX],int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp){
    if(check_null(current_request)){
		return new_request;
	}
	else
		request_queue[*queue_cnt] = new_request;
		(*queue_cnt) = (*queue_cnt) + 1;
		return current_request;
}
//complete sstf
struct RCB handle_request_completion_sstf(struct RCB request_queue[QUEUEMAX],int *queue_cnt,int current_cylinder){
    if(*queue_cnt == 0){
		return empty_rcb;
    }
	struct relative_req{
		struct RCB queue_rcb;
		int distance;
		int close_cylinder;
	};
	
	struct relative_req rcq[*queue_cnt];
	
	int close_req;
	
	for (int i = 0; i < *queue_cnt; i++){
		rcq[i].queue_rcb = request_queue[i];
		rcq[i].close_cylinder = 0;
		rcq[i].distance = (request_queue[i].cylinder - current_cylinder) * (request_queue[i].cylinder - current_cylinder);
		if (i == 0){
			close_req = rcq[i].distance;
        }
		else if (close_req > rcq[i].distance){
			close_req = rcq[i].distance;
        }
	}
	for (int i = 0; i < *queue_cnt; i++){
		if (rcq[i].distance == close_req)
			rcq[i].close_cylinder = 1;
	}
	
	int first_request = 1;
	struct RCB next;
	int early_index;
	
	for (int i = 0; i < *queue_cnt; i++){
		if(first_request == 1 && rcq[i].close_cylinder == 1){
			next = rcq[i].queue_rcb;
			first_request = 0;
			early_index = i;
		}
		else if (rcq[i].close_cylinder == 1 && rcq[i].queue_rcb.arrival_timestamp < next.arrival_timestamp){
			next = rcq[i].queue_rcb;
			early_index = i;
		}
		
		for (int i = early_index; i < *queue_cnt - 1; i++){
			request_queue[i] = request_queue[i+1];
		}
	}
	*queue_cnt = *queue_cnt - 1;
	return next;
}
//arrival look
struct RCB handle_request_arrival_look(struct RCB request_queue[QUEUEMAX],int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp){
    if(check_null(current_request)){
		return new_request;
	}
	else
		request_queue[*queue_cnt] = new_request;
		(*queue_cnt) = (*queue_cnt) + 1;
		return current_request;
}
//complete look
struct RCB handle_request_completion_look(struct RCB request_queue[QUEUEMAX],int *queue_cnt, int current_cylinder, int scan_direction){
    if(*queue_cnt == 0){
		return empty_rcb;
    }
	
	int early = 1000;
	int early_index = 0;
	
	for (int i = 0; i < *queue_cnt; i++){
		if(request_queue[i].cylinder == current_cylinder){
			if(request_queue[i].arrival_timestamp < early){
				early = request_queue[i].arrival_timestamp;
				early_index = i;
			}
		}
	}
	
	if (early != 1000){
		struct RCB result = request_queue[early_index];
		request_queue[early_index] = request_queue[(*queue_cnt) - 1];
		(*queue_cnt)--;
		return result;
	}
	int closest = 1000;
	int closest_index = 0;	
	if(scan_direction == 1){
		for (int i = 0; i < *queue_cnt; i++){
			if(request_queue[i].cylinder > current_cylinder){
				if(request_queue[i].cylinder - current_cylinder < closest){
					closest = request_queue[i].cylinder - current_cylinder;
					closest_index = i;
				}
			}
		}
		
		if (closest != 1000){
			struct RCB result = request_queue[closest_index];
			
			request_queue[closest_index] = request_queue[(*queue_cnt) -1];
			(*queue_cnt)--;
			
			return result;
		}
		
		closest = 1000;
		closest_index = 0;
		
		for (int i = 0; i < *queue_cnt; i++){
			if(request_queue[i].cylinder - current_cylinder < closest){
				closest = request_queue[i].cylinder - current_cylinder;
				closest_index = i;
			}
		}
		struct RCB result = request_queue[early_index];
		request_queue[early_index] = request_queue[(*queue_cnt) - 1];
		(*queue_cnt)--;
		
		return result;	
	}
	
	if (scan_direction == 0){
		for (int i = 0; i < *queue_cnt; i++){
			if(request_queue[i].cylinder < current_cylinder){
				if(request_queue[i].cylinder < current_cylinder){
					closest = request_queue[i].cylinder - current_cylinder;
					closest_index = i;
				}
			}
		}
		
		if (closest != 1000){
			struct RCB result = request_queue[closest_index];
			request_queue[closest_index] = request_queue[(*queue_cnt) - 1];
			(*queue_cnt)--;
			
			return result;
		}
		
		for (int i = 0; i < *queue_cnt; i++){
			if(request_queue[i].cylinder - current_cylinder < closest){
				closest = request_queue[i].cylinder - current_cylinder;
				closest_index = i;
			}
		}
		
		struct RCB result = request_queue[early_index];
		request_queue[early_index] = request_queue[(*queue_cnt) - 1];
		(*queue_cnt)--;
		
		return result;
	}
}