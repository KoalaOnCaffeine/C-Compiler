//
// Created by Tom on 12/7/23.
//

#include <malloc.h>
#include <stddef.h>
#include "map.h"

Map *create_map(long(*hash)(void *)) {
    Map *map = malloc(sizeof(Map));
    map->capacity = DEFAULT_MAP_CAPACITY;
    map->buckets = calloc(map->capacity, sizeof(LinkedList *));
    map->hash = hash;
    return map;
}

void delete_map(Map *map) {
    for (unsigned int i = 0, j = map->capacity; i < j; ++i) {
        delete_linked_list(map->buckets[i]);
    }
    free(map->buckets);
    free(map);
}

void put(Map *map, void *key, void *value) {
    long hash = map->hash(key);
    unsigned long index = hash % map->capacity;

    if (map->buckets[index] == NULL) map->buckets[index] = create_linked_list();

    LinkedList *bucket = map->buckets[index];
    Entry *entry = create_entry(key, value);
    append(bucket, entry);
}

void *get(Map *map, void *key, int *error) {
    long hash = map->hash(key);
    unsigned long index = hash % map->capacity;
    LinkedList *bucket = map->buckets[index];

    Node *current = bucket == NULL ? NULL : bucket->head;

    while (current != NULL) {
        if (((Entry *) current->value)->key == key) {
            if (error != NULL) *error = MAP_RETURN_VALUE;
            return (void *) ((Entry *) current->value)->value;
        }
        current = current->next;
    }
    if (error != NULL) *error = MAP_RETURN_NULL;
    return NULL;
}

void remove_entry(Map *map, void *key, void *value) {
    long hash = map->hash(key);
    unsigned long index = hash % map->capacity;
    LinkedList *bucket = map->buckets[index];

    if (bucket == NULL) return;
    if (bucket->length == 0) return;

    // Handle the head separately
    if (((Entry *) bucket->head->value)->key == key && ((Entry *) bucket->head->value)->value == value) {
        remove_at_index(bucket, 0);
        return;
    }

    Node *prev = bucket->head;

    while (prev->next != NULL) {
        if (((Entry *) prev->next->value)->key == key && ((Entry *) prev->next->value)->value == value) {
            Node *current = prev->next;
            prev->next = current->next;
            delete_node(current);
            --bucket->length;
            break; // If a pair is found, it is guaranteed to be the only possible pair with the key
        }
    }
}
