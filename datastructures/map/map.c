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

    if (!bucket) map->buckets[bucket_index] = bucket = create_bucket();

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
    if (!bucket || !bucket->head) return 0;

    Entry *entry = bucket->head;

    while (entry) {
        if (map->equalsFunction(entry->key, key)) {
            if (entry == bucket->head) bucket->head = entry->next;
            else entry->prev->next = entry->next;
            if (entry == bucket->tail) bucket->tail = entry->prev;
            else entry->next->prev = entry->prev;
            --map->entry_count;

            // Note that this is equivalent to if(entry count < capacity/4), meaning that less than a quarter is filled
            // Don't bother resizing below 16?
            if (4 * map->entry_count < map->capacity && map->capacity > 16)
                resize_map(map, map->capacity / 2);
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
    if (!bucket) return 0; // Failure
    Entry *entry = find_entry(bucket, map->equalsFunction, key);
    if (!entry) return 0; // Failure
    *value_ptr = entry->value;
    return 1; // Success
}

void resize_map(Map *map, unsigned int new_capacity) {
    map->buckets = realloc(map->buckets, new_capacity * sizeof(Bucket *));
    for (unsigned int i = map->capacity; i < new_capacity; i++)
        map->buckets[i] = 0; // Set new bucket pointers to null
    map->capacity = new_capacity;
    rehash(map);
}

void rehash(Map *map) {
    Bucket **new_buckets = calloc(map->capacity, sizeof(Bucket *));
    for (unsigned int i = 0; i < map->capacity; i++) {
        Bucket *old_bucket = map->buckets[i];
        if (!old_bucket) continue;
        Entry *entry = old_bucket->head;
        while (entry) {
            long hash = map->hashFunction(entry->key);
            unsigned int new_bucket_index = hash % map->capacity;
            Bucket *new_bucket = new_buckets[new_bucket_index];
            if (!new_bucket) new_buckets[new_bucket_index] = new_bucket = create_bucket();
            add_entry(new_bucket, entry);
            entry = entry->next;
        }
    }
    free(map->buckets); // Free the old memory, but do not use delete_bucket, as this will delete the entries too
    map->buckets = new_buckets;
}

void *add_entry(Bucket *bucket, Entry *entry) {
    if (bucket->head) {
        bucket->tail->next = entry;
        entry->prev = bucket->tail;
        bucket->tail = entry;
    } else bucket->head = bucket->tail = entry; // If there is no head, there will be no tail

}