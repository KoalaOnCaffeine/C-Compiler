//
// Created by Tom on 12/7/23.
//

#include <malloc.h>
#include <stddef.h>
#include "map.h"

Map *create_map(long(*hash)(void *)) {
    Map *map = malloc(sizeof(Map));
    map->capacity = DEFAULT_MAP_CAPACITY;
    map->buckets = calloc(map->capacity, sizeof(Bucket *));
    map->hash = hash;
    return map;
}

void delete_map(Map *map) {
    for (unsigned int i = 0, j = map->capacity; i < j; ++j) {
        delete_bucket(map->buckets[i]);
    }
    free(map->buckets);
    free(map);
}

void put(Map *map, void *key, void *value) {
    long hash = map->hash(key);
    unsigned long index = hash % map->capacity;

    if (map->buckets[index] == NULL) map->buckets[index] = create_bucket();

    Bucket *bucket = map->buckets[index];
    Entry *entry = create_entry(key, value);
    add_entry(bucket, entry);
}

void *get(Map *map, void *key) {
    long hash = map->hash(key);
    unsigned long index = hash % map->capacity;
    Bucket *bucket = map->buckets[index];
    if (bucket->count == 0) return NULL;

    for (unsigned int i = 0; i < bucket->count; ++i) {
        if (bucket->entries[i]->key == key) return bucket->entries[i]->value;
    }
    return NULL;
}

void remove_entry(Map *map, void *key, void *value) {
    long hash = map->hash(key);
    unsigned long index = hash % map->capacity;
    Bucket *bucket = map->buckets[index];

    if (bucket == NULL) return;

    unsigned int i = 0;
    for (; i < bucket->count; ++i) {
        if (bucket->entries[i]->key == key && bucket->entries[i]->value == value) {
            Entry *entry = bucket->entries[i];
            delete_entry(entry);
            break;
        }
    }
    // Move all entries after the deleted one
    for (; i < bucket->count - 1; ++i) {
        bucket->entries[i] = bucket->entries[i + 1];
    }

    --bucket->count;
}