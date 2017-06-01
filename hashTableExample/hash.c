/*
 **************************************************************************************
 *       Filename:  hash.c
 *    Description:   source file
 *
 *        Version:  1.0
 *        Created:  2017-06-01 13:43:30
 *
 *       Revision:  initial draft;
 **************************************************************************************
 */
#include "hash.h"
#include <stdlib.h>

#define FUNC_ENTER hash_log("Enter\n");
#define FUNC_EXIT hash_log("Exit\n");

uint32_t global_seed = 2976579765;

void hashInit(hash_table * hash_tab)
{
    FUNC_ENTER

    if (!hash_tab)
        return;

    hash_tab->hash_func = MurmurHash3_x86_32;
    hash_tab->key_count = 0;
    hash_tab->hash_array_size = HASH_SIZE;
    hash_tab->hash_array = malloc(hash_tab->hash_array_size * sizeof(*hash_tab->hash_array));
    if (!hash_tab->hash_array) {
        hash_log("Failed to allocate hash table array!");
        return;
    }
    for (uint32_t i = 0; i < hash_tab->hash_array_size; i++) {
        hash_tab->hash_array[i] = NULL;
    }

    FUNC_EXIT
}

void hashDestroy(hash_table * hash_tab)
{
    FUNC_ENTER

    if (!hash_tab)
        return;

    hash_entry * tmp, * entry;
    for (uint32_t i = 0; i < hash_tab->hash_array_size; i++) {
        entry = hash_tab->hash_array[i];
        while (entry != NULL) {
            tmp = entry->next;
            if (entry->key)
                free(entry->key);
            if (entry->value)
                free(entry->value);
            free(entry);
            entry = tmp;
        }
    }

    hash_tab->hash_func = NULL;
    hash_tab->key_count = 0;
    hash_tab->hash_array_size = 0;
    free(hash_tab->hash_array);
    hash_tab->hash_array = NULL;

    FUNC_EXIT
}

void hashInsert(hash_table * hash_tab, void * key, uint32_t key_size, void * value, uint32_t value_size)
{
    FUNC_ENTER

    if (!hash_tab)
        return;
    uint32_t index;
    hash_entry * entry, *tmp;
    hash_tab->hash_func(key, key_size, global_seed, &index);
    index = index % hash_tab->hash_array_size;
    tmp = hash_tab->hash_array[index];
    hash_log("index: %u\n", index);

    // allocate hash entry and fill into key and value
    entry = malloc(sizeof(hash_entry));
    if (!entry) {
        hash_log("Failed to allocate hash entry!");
        return;
    }
    entry->key = malloc(key_size);
    if (!entry->key) {
        hash_log("Failed to allocate hash key!");
        return;
    } else {
        memcpy(entry->key, key, key_size);
    }

    entry->value = malloc(value_size);
    if (!entry->value) {
        hash_log("Failed to allocate hash value!");
        return;
    } else {
        memcpy(entry->value, value, value_size);
    }
    entry->next = NULL;

    // put it into hash table
    if (tmp == NULL) {
        hash_tab->hash_array[index] = entry;
        hash_tab->key_count++;
        return;
    }

    while(tmp->next != NULL) {
        tmp = tmp->next;
    }
    tmp->next = entry;
    hash_tab->key_count++;

    FUNC_EXIT
}

void hashReplace(hash_table * hash_tab, void * key, uint32_t key_size, void * new_value, uint32_t value_size)
{
    FUNC_ENTER

    if (!hash_tab)
        return;
    uint32_t index;
    hash_entry * tmp;
    hash_tab->hash_func(key, key_size, global_seed, &index);
    index = index % hash_tab->hash_array_size;
    tmp = hash_tab->hash_array[index];
    do 
    {
        if (memcmp(tmp->key, key, key_size) == 0) {
            if (tmp->value) {
                hash_log("key: %s old value: %d, new value; %d\n", (char *)key, *(int*)tmp->value, *(int*)new_value);
                free(tmp->value);
            }
            tmp->value = malloc(value_size);
            if (!tmp->value) {
                hash_log("Failed to allocate hash value!");
                return;
            } else {
                memcpy(tmp->value, new_value, value_size);
            }
            break;
        } else {
            tmp = tmp->next;
        }
    } while (tmp->next != NULL);

    FUNC_EXIT
}

void remove_entry(hash_table * hash_tab, hash_entry * entry) {
    hash_log("removing key:%s, value: %d\n", (char *)entry->key, *(int *)entry->value);
    if (entry->key)
        free(entry->key);
    if (entry->value)
        free(entry->value);
    free(entry);
    hash_tab->key_count--;
}

void hashRemove(hash_table * hash_tab, void * key, uint32_t key_size)
{
    FUNC_ENTER

    if (!hash_tab)
        return;
    uint32_t index;
    hash_entry * entry, *prev = NULL;
    hash_tab->hash_func(key, key_size, global_seed, &index);
    index = index % hash_tab->hash_array_size;
    entry = hash_tab->hash_array[index];
    while(entry != NULL)
    {
        if (memcmp(entry->key, key, key_size) == 0) {
            if (prev == NULL)
                /* hash数组里的entry就是我们要找的那个 */
                hash_tab->hash_array[index] = entry->next;
            else
                prev->next = entry->next;
            remove_entry(hash_tab, entry);
            break;
        } else {
            prev = entry;
            entry = entry->next;
        }
    }

    FUNC_EXIT
}

void hashSearch(hash_table * hash_tab, void * key, uint32_t key_size)
{
    FUNC_ENTER

    if (!hash_tab)
        return;
    uint32_t index;
    hash_entry * tmp;
    hash_tab->hash_func(key, key_size, global_seed, &index);
    index = index % hash_tab->hash_array_size;
    tmp = hash_tab->hash_array[index];
    do
    {
        if (memcmp(tmp->key, key, key_size) == 0) {
            hash_log("found key:%s, value: %d\n", (char *)tmp->key, *(int *)tmp->value);
            break;
        } else {
            tmp = tmp->next;
        }
    } while (tmp->next != NULL);

    FUNC_EXIT
}

void hashTraverse(hash_table * hash_tab)
{
    FUNC_ENTER
    
    hash_log("hash table key count: %d\n", hash_tab->key_count);

    hash_entry * tmp, * entry;
    for (uint32_t i = 0; i < hash_tab->hash_array_size; i++) {
        entry = hash_tab->hash_array[i];
        while (entry != NULL) {
            tmp = entry->next;
            if (entry->key && entry->value) {
                hash_log("hash index: %u, key: %s, value; %d\n",
                        i, (char *)entry->key, *(int *)entry->value); 
            }
            entry = tmp;
        }
    }

    FUNC_EXIT
}

/********************************** END **********************************************/

