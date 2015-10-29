#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct linked_list_node {
    int value;
    int next_key;
    int self_key;
} linked_list_node;

typedef struct linked_list {
    int head_key;
    int tail_key;
    int self_key;
    int size;
} linked_list;

void add_end_of_list(int list_key, int new_value){
    int list_id = shmget(list_key, 0, 0);
	struct linked_list * list = (struct linked_list *)shmat(list_id, 0, 0);
    
    int new_node_key;
    if(list->size > 1){
        new_node_key = list->tail_key + 1;
    } else if (list->size == 1){
        new_node_key = list->head_key + 1;
    } else {
        new_node_key = list->self_key + 1;
    }
    int new_node_id = shmget(new_node_key, sizeof(struct linked_list_node), 0777 | IPC_CREAT);
    struct linked_list_node * newnode = (struct linked_list_node *)shmat(new_node_id, 0, 0);
    
    newnode->value = new_value;
    newnode->next_key = -1;
    if(list->size == 0){
        //if the list is empty
        list->head_key = new_node_key;
    } else if(list->size == 1){
        //if the list only had one node in it
        int head_id = shmget(list->head_key, sizeof(struct linked_list_node), 0777 | IPC_CREAT);
        struct linked_list_node * head = (struct linked_list_node *)shmat(head_id, 0, 0);
        head-> next_key = new_node_key;
        list->tail_key = new_node_key;
    } else {
        //The list has two or more nodes in it
        int tail_id = shmget(list->tail_key, sizeof(struct linked_list_node), 0777 | IPC_CREAT);
        struct linked_list_node * tail = (struct linked_list_node *)shmat(tail_id, 0, 0);
        tail->next_key = new_node_key;
        list->tail_key = new_node_key;
    }
    list->size = list->size + 1;
    return;
}

void delete_first_request(int list_key){
    int list_id = shmget(list_key, 0, 0);
	struct linked_list * list = (struct linked_list *)shmat(list_id, 0, 0);
    linked_list_node * to_delete = list->head;
    if(to_delete != NULL){
        list->head = list->head->next;
        free(to_delete); 
    }
    list->size = list->size - 1;
    return;
}

int first_request_amount(int list_key){
    int list_id = shmget(list_key, 0, 0);
	struct linked_list * list = (struct linked_list *)shmat(list_id, 0, 0);
    printf("head key: %d\n", list->head_key);
    printf("size: %d\n", list->size);
    if(list->head_key != -1){
        int head_id = shmget(list->head_key, 0, 0);
        struct linked_list_node * head = (struct linked_list_node *)shmat(head_id, 0, 0);
        return head->value;
    } else {
        return -1;
    }
}
/*
void release_linked_list(int list_key){
    int list_id = shmget(list_key, 0, 0);
	struct linked_list * list = (struct linked_list *)shmat(list_id, 0, 0);
    while(list->head_key != -1){
        delete_first_request(list_key);
    }
    shmctl(list_id, IPC_RMID, 0);
}*/