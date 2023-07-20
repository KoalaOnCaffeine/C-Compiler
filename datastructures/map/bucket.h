//
// Created by Tom on 12/7/23.
//

#ifndef C_COMPILER_BUCKET_H
#define C_COMPILER_BUCKET_H

#include "entry.h"

typedef struct {
    unsigned int count; // The number of entries in the bucket
    Entry **entries; // The array of entry pointers
} Bucket;

Bucket *create_bucket();

void delete_bucket(Bucket *bucket);

void add_entry(Bucket *bucket, Entry *entry);

#endif //C_COMPILER_BUCKET_H
