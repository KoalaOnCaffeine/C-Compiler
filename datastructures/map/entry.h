//
// Created by Tom on 12/7/23.
//

#ifndef C_COMPILER_ENTRY_H
#define C_COMPILER_ENTRY_H

typedef struct {
    void *key, *value;
} Entry;

Entry *create_entry(void *key, void *value);

void delete_entry(Entry *entry);

#endif //C_COMPILER_ENTRY_H
