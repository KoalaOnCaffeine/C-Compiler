//
// Created by Tom on 13/7/23.
//

#include <malloc.h>
#include <stddef.h>
#include "linkedlist.h"

LinkedList *create_linked_list() {
    LinkedList *linkedList = malloc(sizeof(LinkedList));
    linkedList->head = NULL;
    linkedList->length = 0;

    return linkedList;
}

void delete_linked_list(LinkedList *linkedList) {
    if (linkedList == NULL) return;
    Node *next = linkedList->head;
    while (next != NULL) {
        Node *current = next;
        next = next->next;
        free(current);
    }
    free(linkedList);
}

Node *create_node(void *value) {
    Node *node = malloc(sizeof(Node));
    node->value = value;
    node->next = NULL;
    return node;
}

void delete_node(Node *node) {
    free(node);
}

void append(LinkedList *linkedList, void *value) {
    Node *node = create_node(value);
    if (linkedList->head == NULL) {
        linkedList->head = node;
    } else {
        Node *current = linkedList->head;
        while (current->next != NULL) current = current->next;
        current->next = node;
    }
    ++linkedList->length;
}

void prepend(LinkedList *linkedList, void *value) {
    Node *node = create_node(value);
    Node *head = linkedList->head;
    linkedList->head = node;
    node->next = head;
    ++linkedList->length;
}

void insert(LinkedList *linkedList, void *value, unsigned int index) {
    if (index == 0) return prepend(linkedList, value);
    Node *previous = linkedList->head;
    for (unsigned int i = 0; i < index - 1; ++i) {
        if (previous == NULL) return;
        previous = previous->next;
    }
    ++linkedList->length;
}

void set_index(LinkedList *linkedList, void *value, unsigned int index) {
    Node *current = linkedList->head;
    for (unsigned int i = 0; i < index; ++i) {
        if (current == NULL) return;
        current = current->next;
    }
    current->value = value;
}

int remove_value(LinkedList *linkedList, void *value) {
    if (linkedList->length == 0) return 0;
    unsigned int removed = 0;

    Node *prev = linkedList->head; // Check the head last

    while (prev->next != NULL) {
        if (prev->next->value == value) {
            Node *current = prev->next;
            prev->next = current->next;
            delete_node(current);
            ++removed;
        }
    }

    if (linkedList->head->value == value) {
        Node *head = linkedList->head;
        linkedList->head = head->next;
        delete_node(head);
        ++removed;
    }

    linkedList->length -= removed;

    return removed != 0;
}

int remove_at_index(LinkedList *linkedList, unsigned int index) {
    if (linkedList->length == 0 || index >= linkedList->length) return 0; // Nothing to remove
    if (index == 0) {
        Node *head = linkedList->head;
        linkedList->head = head->next;
        delete_node(head);
    } else {
        Node *prev = linkedList->head;
        for (unsigned int i = 0; i < index - 1; ++i) {
            prev = prev->next;
        }
        Node *next = prev->next->next;
        delete_node(prev->next);
        prev->next = next;
    }
    --linkedList->length;
    return 1; // Node was removed
}
