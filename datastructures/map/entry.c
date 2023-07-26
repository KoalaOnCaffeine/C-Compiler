//
// Created by Tom on 12/7/23.
//

#include <malloc.h>
#include <stddef.h>
#include "entry.h"

Entry *create_entry(void *key, void *value) {
    Entry *entry = malloc(sizeof(Entry));
    entry->key = key;
    entry->value = value;
    return entry;
}

void delete_entry(Entry *entry) {
    if (entry != NULL) free(entry);
}
