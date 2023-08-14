//
// Created by Tom on 29/7/23.
//

#include <malloc.h>
#include "map.h"

Map *create_map(equals_function equalsFunction, hash_function hashFunction) {
    Map *map = calloc(1, sizeof(Map));
    /*
     * Initialize capacity and entry_count to 0
     * Initialize buckets to null
     */
    map->equalsFunction = equalsFunction;
    map->hashFunction = hashFunction;
    return map;
}

void delete_map(Map *map) {
    if (!map) return;
    for (unsigned int i = 0; i < map->capacity; ++i)
        delete_bucket(map->buckets[i]);
    free(map);
}

Bucket *create_bucket() {
    Bucket *bucket = calloc(1, sizeof(Bucket));
    return bucket;
}

void delete_bucket(Bucket *bucket) {
    Entry *cur = bucket->head;
    while (cur) {
        Entry *tmp = cur;
        cur = cur->next;
        delete_entry(tmp);
    }
    free(bucket);
}

Entry *create_entry(void *key, void *value) {
    Entry *entry = calloc(1, sizeof(Entry)); // Initialize prev and next pointers to null
    entry->key = key;
    entry->value = value;
    return entry;
}

void delete_entry(Entry *entry) {
    free(entry);
}

void put(Map *map, void *key, void *value) {
    // Resize before adding
    if (4 * map->entry_count >= 3 * map->capacity)
        resize_map(map, map->capacity ? map->capacity * 2 : 16); // Double, or initially 16

    long hash = map->hashFunction(key);
    unsigned int bucket_index = hash % map->capacity; // Capacity will always be larger than 0
    Bucket *bucket = map->buckets[bucket_index];

    Entry *entry = find_entry(bucket, map->equalsFunction, key);
    if (entry)
        return (void) (entry->value = value); // Overriding an existing entry, entry count does not increase

    // Create a new entry
    entry = create_entry(key, value);
    ++map->entry_count;
    add_entry(bucket, entry);
}

Entry *remove_key(Map *map, void *key) {
    if (!map->capacity) return 0;
    long hash = map->hashFunction(key);
    unsigned int bucket_index = hash % map->capacity;
    Bucket *bucket = map->buckets[bucket_index];
    if (!bucket->head) return 0;

    Entry *entry = bucket->head;

    while (entry) {
        if (map->equalsFunction(entry->key, key)) {
            remove_entry(bucket, entry);
            --map->entry_count;
            return entry;
        }
        entry = entry->next;
    }

    return 0;
}

Entry *find_entry(Bucket *bucket, equals_function equalsFunction, void *key) {
    Entry *cur = bucket->head;
    while (cur) {
        if (equalsFunction(cur->key, key)) return cur;
        else cur = cur->next;
    }
    return 0;
}

int get(Map *map, void *key, void **value_ptr) {
    if (!map->capacity) return 0; // Failure
    long hash = map->hashFunction(key);
    unsigned int bucketIndex = hash % map->capacity;
    Bucket *bucket = map->buckets[bucketIndex];
    Entry *entry = find_entry(bucket, map->equalsFunction, key);
    if (!entry) return 0; // Failure
    *value_ptr = entry->value;
    return 1; // Success
}

void resize_map(Map *map, unsigned int new_capacity) {
    map->buckets = realloc(map->buckets, new_capacity * sizeof(Bucket *));
    for (unsigned int i = map->capacity; i < new_capacity; i++) map->buckets[i] = create_bucket();
    map->capacity = new_capacity;
    rehash(map);
}

void rehash(Map *map) {
    for (unsigned int i = 0; i < map->capacity; i++) {
        Bucket *old_bucket = map->buckets[i];
        Entry *cur = old_bucket->head;
        while (cur) {
            long hash = map->hashFunction(cur->key);
            unsigned int new_bucket_index = hash % map->capacity;
            if (new_bucket_index != i) {
                Bucket *new_bucket = map->buckets[new_bucket_index];
                remove_entry(old_bucket, cur);
                add_entry(new_bucket, cur);
            }
            cur = cur->next;
        }
    }
}

void add_entry(Bucket *bucket, Entry *entry) {
    if (bucket->head) {
        bucket->tail->next = entry;
        entry->prev = bucket->tail;
        bucket->tail = entry;
    } else bucket->head = bucket->tail = entry; // If there is no head, there will be no tail
}

void remove_entry(Bucket *bucket, Entry *entry) {
    if (bucket->head == entry) bucket->head = entry->next;
    else entry->prev->next = entry->next;
    if (bucket->tail == entry) bucket->tail = entry->prev;
    else entry->next->prev = entry->prev;
}

void foreach_key(Map *map, void (for_each)(void *key)) {
    for (unsigned int i = 0; i < map->capacity; i++) {
        Entry *cur = map->buckets[i]->head;
        while (cur) {
            for_each(cur->key);
            cur = cur->next;
        }
    }
}

void foreach_value(Map *map, void (for_each)(void *value)) {
    for (unsigned int i = 0; i < map->capacity; i++) {
        Entry *cur = map->buckets[i]->head;
        while (cur) {
            for_each(cur->value);
            cur = cur->next;
        }
    }
}

void foreach_entry(Map *map, void(for_each)(void *key, void *value)) {
    for (unsigned int i = 0; i < map->capacity; i++) {
        Entry *cur = map->buckets[i]->head;
        while (cur) {
            for_each(cur->key, cur->value);
            cur = cur->next;
        }
    }
}