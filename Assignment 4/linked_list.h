#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct linked_list_node {
    int value;
    struct linked_list_node * next;
} linked_list_node;

typedef struct linked_list {
    struct linked_list_node * head;
    struct linked_list_node * tail;
} linked_list;

void add_end_of_list(linked_list * list, int new_value){
    struct linked_list_node * newnode = (struct linked_list_node *)malloc(sizeof(struct linked_list_node));
    newnode->value = new_value;
    newnode->next = NULL;
    if(list->head == NULL){
        //if the list is empty
        list->head = newnode;
    } else if(list->tail == NULL){
        //if the list only had one node in it
        list->head-> next = newnode;
        list->tail = newnode;
    } else {
        list->tail->next = newnode;
        list->tail = newnode;
    }
    return;
}

void delete_first_request(linked_list * list){
    linked_list_node * to_delete = list->head;
    if(to_delete != NULL){
        list->head = list->head->next;
        free(to_delete); 
    }
    return;
}

int first_request_amount(linked_list * list){
    if(list->head != NULL){
        return list->head->value;
    } else {
        return -1;
    }
}