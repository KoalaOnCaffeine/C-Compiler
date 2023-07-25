//
// Created by Tom on 12/7/23.
//

#include "entry.h"
#include "../linkedlist/linkedlist.h"

#ifndef DEFAULT_MAP_CAPACITY
#define DEFAULT_MAP_CAPACITY 16
#endif
#ifndef MAP_RETURN_NULL
#define MAP_RETURN_NULL 0
#endif
#ifndef MAP_RETURN_VALUE
#define MAP_RETURN_VALUE 1
#endif

#ifndef C_COMPILER_MAP_H
#define C_COMPILER_MAP_H

/*
 * The map is a list of entries
 * Buckets store multiple entries which all have colliding hashes
 */

typedef struct {
    unsigned int capacity; // The number of buckets in the map
    LinkedList **buckets; // The array of bucket pointers

    long (*hash)(void *); // A hashing function
} Map;

Map *create_map(long(*hash)(void *));

void delete_map(Map *map);

void put(Map *map, void *key, void *value);

int get(Map *map, void *key, void **value);

void remove_entry(Map *map, void *key, void *value);

#endif //C_COMPILER_MAP_H
