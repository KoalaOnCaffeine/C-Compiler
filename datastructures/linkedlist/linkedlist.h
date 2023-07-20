//
// Created by Tom on 13/7/23.
//

#ifndef C_COMPILER_LINKEDLIST_H
#define C_COMPILER_LINKEDLIST_H

typedef struct Node Node;
typedef struct LinkedList LinkedList;

struct Node {
    void *value;
    Node *next;
};

struct LinkedList {
    Node *head;
    unsigned int length;
};

LinkedList *create_linked_list();

void delete_linked_list(LinkedList *linkedList);

Node *create_node(void *value);

void delete_node(Node *node);

void append(LinkedList *linkedList, void *value);

void prepend(LinkedList *linkedList, void *value);

void insert(LinkedList *linkedList, void *value, unsigned int index);

void set_index(LinkedList *linkedList, void *value, unsigned int index);

int remove_value(LinkedList *linkedList, void *value); // Returns whether it was removed

int remove_at_index(LinkedList *linkedList, unsigned int index); // Returns whether it was removed

#endif //C_COMPILER_LINKEDLIST_H
