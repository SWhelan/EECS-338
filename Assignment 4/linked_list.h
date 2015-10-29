/*
 * Sarah Whelan
 * slw96
 * 10/29/2015
 *
 */
 
// A singly linked list node
typedef struct linked_list_node {
    int value;
    int next_key;
    int self_key;
} linked_list_node;

/* 
 * A singly linked list 
 * stores info to get to head and tail of list for
 * convenience as it is a list in shared memory
 * keys to get the shared memory are stored not
 * pointers to actual nodes
 */
typedef struct linked_list {
    int head_key;
    int tail_key;
    int self_key;
    int size;
} linked_list;

// Add a node to the end of the list
void add_end_of_list(int list_key, int new_value){
    // Get the list from shared memory
    int list_id = shmget(list_key, 0, 0);
    if(list_id == -1){
        printf("There was an error getting the linked list. Aborting.\n");
        return;
    }
	struct linked_list * list = (struct linked_list *)shmat(list_id, 0, 0);
    if(list == (void *) -1){
        printf("There was an error getting the linked list. Aborting.\n");
        fprintf(stderr, "errno %d: %s\n", errno, strerror(errno));
    }
    // Determine the next shared memory key to use for the new node
    int new_node_key;
    if(list->size > 1){
        new_node_key = list->tail_key + 1;
    } else if (list->size == 1){
        new_node_key = list->head_key + 1;
    } else {
        new_node_key = list->self_key + 1;
    }
    // Make a new node in shared memory
    int new_node_id = shmget(new_node_key, sizeof(struct linked_list_node), 0777 | IPC_CREAT);
    struct linked_list_node * newnode = (struct linked_list_node *)shmat(new_node_id, 0, 0);
    // Store the value in the new node
    newnode->value = new_value;
    newnode->next_key = -1;
    
    // Update the keys in the appropriate places to add it to the list
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
    /* Keeping track of size inside the linked list to avoid
     * passing in wcount to all of the linked_list method 
     * as now this linked list could be used for other 
     * purposes
     */
    list->size = list->size + 1;
    return;
}


// Delete the head of the list.
void delete_first_request(int list_key){
    int list_id;
    struct linked_list * list;
    int head_id;
    struct linked_list_node * to_delete;
    
    // Get the list from shared memory
    list_id = shmget(list_key, 0, 0);
    if(list_id == -1){
        printf("There was an error getting the linked list. Aborting.\n");
        return;
    }
	list = (struct linked_list *)shmat(list_id, 0, 0);
    if(list == (void *) -1){
        printf("There was an error getting the linked list. Aborting.\n");
        fprintf(stderr, "errno %d: %s\n", errno, strerror(errno));
    }
    
    // Nothing to delete
    if(list->size == 0){
        return;
    }
    
    // Get the head node from shared memory
    head_id = shmget(list->head_key, 0, 0);
    to_delete = (struct linked_list_node *)shmat(head_id, 0, 0);

    // Update the keys accordingly
    if(list->size == 1){
        list->head_key = -1;
        list->tail_key = -1;
    } else if(list->size == 2){
        list->head_key = list->tail_key;
        list->tail_key = -1;
    } else {
        list->head_key = to_delete->next_key;
    }
    
    // Again keeping track of size for convenience 
    list->size = list->size - 1;
    
    // Release the shared memory for the old head node
    shmctl(head_id, IPC_RMID, 0);
    return;
}

// Get the first value in the linked list
int first_request_amount(int list_key){
    int list_id;
    struct linked_list * list;
    int head_id;
    struct linked_list_node * head;
    
    // Get the list from shared memory
    list_id = shmget(list_key, 0, 0);
    if(list_id == -1){
        printf("There was an error getting the linked list. Aborting.\n");
        return -1;
    }
	list = (struct linked_list *)shmat(list_id, 0, 0);
    if(list == (void *) -1){
        printf("There was an error getting the linked list. Aborting.\n");
        fprintf(stderr, "errno %d: %s\n", errno, strerror(errno));
    }
    
    if(list->size > 0){
        // Get the head node from shared memory
        head_id = shmget(list->head_key, 0, 0);
        head = (struct linked_list_node *)shmat(head_id, 0, 0);
        
        // Return its value
        return head->value;
    } else {
        // There was nothing in the list return a dummy value
        return -1;
    }
}

// Release all nodes of the list from shared memory
void release_linked_list(int list_key){
    int list_id;
    struct linked_list * list;
    // Get the list from shared memory
    list_id = shmget(list_key, 0, 0);
    if(list_id == -1){
        printf("There was an error getting the linked list. Aborting.\n");
        return;
    }
	list = (struct linked_list *)shmat(list_id, 0, 0);
    if(list == (void *) -1){
        printf("There was an error getting the linked list. Aborting.\n");
        fprintf(stderr, "errno %d: %s\n", errno, strerror(errno));
    }
    
    // While there are nodes
    while(list->size > 0){
        // remove the first node
        delete_first_request(list_key);
    }
    
    // Release the linked list container from shared memory
    shmctl(list_id, IPC_RMID, 0);
    return;
}