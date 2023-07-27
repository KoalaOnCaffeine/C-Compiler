//
// Created by Tom on 12/7/23.
//

#include <malloc.h>
#include <stddef.h>
#include "map.h"

Map *create_map(int(*equals)(void *, void *), long(*hash)(void *)) {
    Map *map = malloc(sizeof(Map));
    map->capacity = DEFAULT_MAP_CAPACITY;
    map->buckets = calloc(map->capacity, sizeof(LinkedList *));
    map->equals = equals;
    map->hash = hash;
    return map;
}

void delete_map(Map *map) {
    if (map == NULL) return;
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

int get(Map *map, void *key, void **value) {
    long hash = map->hash(key);
    unsigned long index = hash % map->capacity;
    LinkedList *bucket = map->buckets[index];

    Node *current = bucket ? bucket->head : NULL;

    for (; current; current = current->next) {
        if (map->equals(((Entry *) current->value)->key, key)) {
            *value = (void *) ((Entry *) current->value)->value;
            return MAP_RETURN_VALUE;
        }
    }
    return MAP_RETURN_NULL;
}

void remove_key(Map *map, void *key) {
    long hash = map->hash(key);
    unsigned long index = hash % map->capacity;
    LinkedList *bucket = map->buckets[index];

    if (bucket == NULL) return;

    // No need to check bucket length, since if the bucket is not null, there must be an entry
    Node *prev = bucket->head;

    // Handle the head separately
    if (map->equals(((Entry *) prev->value)->key, key)) {
        remove_at_index(bucket, 0);
    } else {
        while (prev->next != NULL) {
            if (map->equals(((Entry *) prev->next->value)->key, key)) {
                Node *current = prev->next;
                prev->next = current->next;
                delete_node(current);
                --bucket->length;
                break; // If a pair is found, it is guaranteed to be the only possible pair with the key
            }
        }
    }

    // Delete the bucket, remove the pointer in the list of buckets
    if (!bucket->length) {
        map->buckets[index] = NULL;
        delete_linked_list(bucket);
    }

}
