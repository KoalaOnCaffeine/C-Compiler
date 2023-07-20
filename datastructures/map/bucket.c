//
// Created by Tom on 12/7/23.
//

#include <malloc.h>
#include "bucket.h"

Bucket *create_bucket() {
    Bucket *bucket = malloc(sizeof(Bucket));
    bucket->count = 0;
    bucket->entries = malloc(0);
    return bucket;
}

void delete_bucket(Bucket *bucket) {
    for (unsigned int i = 0; i < bucket->count; ++i) {
        delete_entry(bucket->entries[i]);
    }
    free(bucket->entries);
    free(bucket);
}

void add_entry(Bucket *bucket, Entry *entry) {
    if (bucket->count == 0) bucket->entries = malloc(sizeof(Entry *));
    else bucket->entries = realloc(bucket->entries, sizeof(Entry *) * (bucket->count + 1));
    bucket->entries[bucket->count] = entry;
    ++bucket->count;
}