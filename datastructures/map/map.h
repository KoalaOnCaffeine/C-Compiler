//
// Created by Tom on 29/7/23.
//

#ifndef C_COMPILER_MAP_H
#define C_COMPILER_MAP_H

typedef struct Map Map;
typedef struct Bucket Bucket;
typedef struct Entry Entry;

typedef int (*equals_function)(void *, void *);

typedef long (*hash_function)(void *);

struct Map {
    unsigned int capacity; // The number of bucket
    unsigned int entry_count; // The number of entries
    Bucket **buckets; // Array of buckets
    equals_function equalsFunction;
    hash_function hashFunction;
};

struct Bucket {
    Entry *head; // The first entry in the bucket
    Entry *tail; // The last entry in the bucket - used for faster additions
};

struct Entry {
    void *key; // The key associated with the entry
    void *value; // The value associated with the entry
    Entry *prev; // The previous entry
    Entry *next; // The next entry
};

// Memory functions

Map *create_map(equals_function equalsFunction, hash_function hashFunction);

// Deletes a map and all associated memory
void delete_map(Map *map);

// Creates an empty bucket with no head or tail
Bucket *create_bucket();

// Deletes a bucket and all associated memory
void delete_bucket(Bucket *bucket);

// Creates an empty entry with a null pointer key, value and previous and next entries
Entry *create_entry(void *key, void *value);

void delete_entry(Entry *entry);

// Map functions

// Associates the key to a value
void put(Map *map, void *key, void *value);

// Gets the associated value from a given key, and returns whether a value was found
int get(Map *map, void *key, void **value_ptr);

// Disassociates a value from a key, returning the removed entry
Entry *remove_key(Map *map, void *key);

// Finds an entry in a bucket, given a key. Returns the entry
Entry *find_entry(Bucket *bucket, equals_function equalsFunction, void *key); // Helper function

// Add an entry to a bucket
void *add_entry(Bucket *bucket, Entry *entry);

// Change the size of the map (note that this should call rehash immediately after resizing)
void resize_map(Map *map, unsigned int new_capacity);

// Recalculate the buckets for each entry in the map
void rehash(Map *map);

#endif //C_COMPILER_MAP_H
