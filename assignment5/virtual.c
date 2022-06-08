#include "oslabs.h"
typedef struct PTE PTE;

int process_page_access_fifo(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp){
    if (page_table[page_number].is_valid){
		page_table[page_number].last_access_timestamp = current_timestamp;
		page_table[page_number].reference_count++;
		return page_table[page_number].frame_number;
	}

	if (*frame_cnt > 0){
		page_table[page_number].is_valid = 1;
		page_table[page_number].frame_number = frame_pool[(*frame_cnt)--];
		page_table[page_number].arrival_timestamp = current_timestamp;
		page_table[page_number].last_access_timestamp = current_timestamp;
		page_table[page_number].reference_count = 1;
		return page_table[page_number].frame_number;
	}

	int index = -1;
	int arrival_timestamp = 2147483647;
	for (int i = 0; i < *table_cnt; i++){
		if (page_table[i].is_valid &&
			page_table[i].arrival_timestamp < arrival_timestamp){
			arrival_timestamp = page_table[i].arrival_timestamp;
			index = i;
		}
	}
	page_table[page_number].frame_number = page_table[index].frame_number;
	page_table[page_number].is_valid = 1;
	page_table[page_number].arrival_timestamp = current_timestamp;
	page_table[page_number].last_access_timestamp = current_timestamp;
	page_table[page_number].reference_count = 1;

	page_table[index] = (PTE){0, -1, -1, -1};

	return page_table[page_number].frame_number;
} 
int count_page_faults_fifo(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt){
    int count = 0;
	for (int i = 0; i < reference_cnt; i++){
		int page_number = refrence_string[i];
		if (page_table[page_number].is_valid){
			page_table[page_number].last_access_timestamp = i;
			page_table[page_number].reference_count++;
			continue;
		}
		count++;
		if (frame_cnt > 0){
			page_table[page_number].is_valid = 1;
			page_table[page_number].frame_number = frame_pool[frame_cnt--];
			page_table[page_number].arrival_timestamp = i;
			page_table[page_number].last_access_timestamp = i;
			page_table[page_number].reference_count = 1;
			continue;
		}

		int index = -1;
		int arrival_timestamp = 2147483647;
		for (int i = 0; i < table_cnt; i++){
			if (page_table[i].is_valid && page_table[i].arrival_timestamp < arrival_timestamp){
				arrival_timestamp = page_table[i].arrival_timestamp;
				index = i;
			}
		}
		page_table[page_number].frame_number = page_table[index].frame_number;
		page_table[page_number].is_valid = 1;
		page_table[page_number].arrival_timestamp = i;
		page_table[page_number].last_access_timestamp = i;
		page_table[page_number].reference_count = 1;
		page_table[index] = (PTE){0, -1, -1, -1};
	}

	return count;
}
int process_page_access_lru(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp){
    if (page_table[page_number].is_valid){
		page_table[page_number].last_access_timestamp = current_timestamp;
		page_table[page_number].reference_count++;
		return page_table[page_number].frame_number;
	}

	if (*frame_cnt > 0){
		page_table[page_number].is_valid = 1;
		page_table[page_number].frame_number = frame_pool[(*frame_cnt)--];
		page_table[page_number].arrival_timestamp = current_timestamp;
		page_table[page_number].last_access_timestamp = current_timestamp;
		page_table[page_number].reference_count = 1;
		return page_table[page_number].frame_number;
	}

	int index = -1;
	int last_access_timestamp = 2147483647;
	for (int i = 0; i < *table_cnt; i++){
		if (page_table[i].is_valid && page_table[i].last_access_timestamp < last_access_timestamp){
			last_access_timestamp = page_table[i].last_access_timestamp;
			index = i;
		}
	}
	page_table[page_number].frame_number = page_table[index].frame_number;
	page_table[page_number].is_valid = 1;
	page_table[page_number].arrival_timestamp = current_timestamp;
	page_table[page_number].last_access_timestamp = current_timestamp;
	page_table[page_number].reference_count = 1;
	page_table[index] = (PTE){0, -1, -1, -1};

	return page_table[page_number].frame_number;
}
int count_page_faults_lru(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt){
    int count = 0;
	for (int i = 0; i < reference_cnt; i++){
		int page_number = refrence_string[i];
		if (page_table[page_number].is_valid){
			page_table[page_number].last_access_timestamp = i;
			page_table[page_number].reference_count++;
			continue;
		}
		count++;

		if (frame_cnt > 0){
			page_table[page_number].is_valid = 1;
			page_table[page_number].frame_number = frame_pool[frame_cnt--];
			page_table[page_number].arrival_timestamp = i;
			page_table[page_number].last_access_timestamp = i;
			page_table[page_number].reference_count = 1;
			continue;
		}

		int index = -1;
		int last_access_timestamp = 2147483647;
		for (int i = 0; i < table_cnt; i++){
			if (page_table[i].is_valid && page_table[i].last_access_timestamp < last_access_timestamp){
				last_access_timestamp = page_table[i].last_access_timestamp;
				index = i;
			}
		}
		page_table[page_number].frame_number = page_table[index].frame_number;
		page_table[page_number].is_valid = 1;
		page_table[page_number].arrival_timestamp = i;
		page_table[page_number].last_access_timestamp = i;
		page_table[page_number].reference_count = 1;
		page_table[index] = (PTE){0, -1, -1, -1};
	}

	return count;
}
int process_page_access_lfu(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp){
    if (page_table[page_number].is_valid){
		page_table[page_number].last_access_timestamp = current_timestamp;
		page_table[page_number].reference_count++;
		return page_table[page_number].frame_number;
	}

	if (*frame_cnt > 0){
		page_table[page_number].is_valid = 1;
		page_table[page_number].frame_number = frame_pool[(*frame_cnt)--];
		page_table[page_number].arrival_timestamp = current_timestamp;
		page_table[page_number].last_access_timestamp = current_timestamp;
		page_table[page_number].reference_count = 1;
		return page_table[page_number].frame_number;
	}

	int index = -1;
	int reference_count = 2147483647;
	int arrival_timestamp = 2147483647;
	for (int i = 0; i < *table_cnt; i++){
		if (page_table[i].is_valid && page_table[i].reference_count <= reference_count){
			reference_count = page_table[i].reference_count;
			if (page_table[i].arrival_timestamp < arrival_timestamp){
				arrival_timestamp = page_table[i].arrival_timestamp;
				index = i;
			}
		}
	}
	page_table[page_number].frame_number = page_table[index].frame_number;
	page_table[page_number].is_valid = 1;
	page_table[page_number].arrival_timestamp = current_timestamp;
	page_table[page_number].last_access_timestamp = current_timestamp;
	page_table[page_number].reference_count = 1;
	page_table[index] = (PTE){0, -1, -1, -1};

	return page_table[page_number].frame_number;
}
int count_page_faults_lfu(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt){
    int count = 0;
	for (int i = 0; i < reference_cnt; i++){
		int page_number = refrence_string[i];
		if (page_table[page_number].is_valid){
			page_table[page_number].last_access_timestamp = i;
			page_table[page_number].reference_count++;
			continue;
		}
		count++;

		if (frame_cnt > 0){
			page_table[page_number].is_valid = 1;
			page_table[page_number].frame_number = frame_pool[frame_cnt--];
			page_table[page_number].arrival_timestamp = i;
			page_table[page_number].last_access_timestamp = i;
			page_table[page_number].reference_count = 1;
			continue;
		}

		int index = -1;
		int reference_count = 2147483647;
		int arrival_timestamp = 2147483647;
		for (int i = 0; i < table_cnt; i++){
			if (page_table[i].is_valid && page_table[i].reference_count <= reference_count){
				reference_count = page_table[i].reference_count;
				if (page_table[i].arrival_timestamp < arrival_timestamp){
					arrival_timestamp = page_table[i].arrival_timestamp;
					index = i;
				}
			}
		}
		page_table[page_number].frame_number = page_table[index].frame_number;
		page_table[page_number].is_valid = 1;
		page_table[page_number].arrival_timestamp = i;
		page_table[page_number].last_access_timestamp = i;
		page_table[page_number].reference_count = 1;
		page_table[index] = (PTE){0, -1, -1, -1};
	}

	return count;
}