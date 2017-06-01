/*
 **************************************************************************************
 *       Filename:  hash.h
 *    Description:   header file
 *
 *        Version:  1.0
 *        Created:  2017-06-01 12:45:47
 *
 *       Revision:  initial draft;
 **************************************************************************************
 */

#ifndef HASH_H_INCLUDED
#define HASH_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include "murmur.h"

#define hash_log(msg, ...) printf("[FUNC: %s: LINE:%d]\t" msg, __func__, __LINE__, ##__VA_ARGS__)

#define HASH_SIZE 32

typedef unsigned int uint32_t;

struct hashEntry {
    void * key;
    void * value;
    struct hashEntry * next;
};

typedef struct hashEntry hash_entry;

struct hashTable {
    MURMUR_FUNC* hash_func;
    hash_entry ** hash_array;
    uint32_t hash_array_size;
    uint32_t key_count;
};

typedef struct hashTable hash_table;

void hashInit(hash_table * hash_tab);
void hashDestroy(hash_table * hash_tab);
void hashInsert(hash_table * hash_tab, void * key, uint32_t key_size, void * value, uint32_t value_size);
void hashReplace(hash_table * hash_tab, void * key, uint32_t key_size, void * new_value, uint32_t value_size);
void hashRemove(hash_table * hash_tab, void * key, uint32_t key_size);
void hashSearch(hash_table * hash_tab, void * key, uint32_t key_size);
void hashTraverse(hash_table * hash_tab);

#endif /*HASH_H_INCLUDED*/

/********************************** END **********************************************/
