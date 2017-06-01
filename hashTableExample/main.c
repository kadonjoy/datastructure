/*
 **************************************************************************************
 *       Filename:  main.c
 *    Description:   source file
 *
 *        Version:  1.0
 *        Created:  2017-06-01 13:53:16
 *
 *       Revision:  initial draft;
 **************************************************************************************
 */
#include "hash.h"

int main(int argc __attribute__((unused)), char** argv[] __attribute__((unused))) {
    hash_table ht_instance;

    hash_log("Init hash table instance...\n");
    memset(&ht_instance, 0, sizeof(ht_instance));
    hashInit(&ht_instance);
    char *name = "kadon";
    uint32_t age = 26;
    uint32_t new_age = 30;
    char *name_array[] = {"messi", "sussi", "kadonjoy", "xiva", "xiaoming", "donglu"};
    uint32_t age_array[] = {30, 30, 26, 37, 12, 48};

    hashInsert(&ht_instance, name, strlen(name) + 1, &age, sizeof(age));
    hashReplace(&ht_instance, name, strlen(name) + 1, &new_age, sizeof(new_age));
#if 1 
    for (uint32_t i = 0; i < sizeof(age_array)/sizeof(age_array[0]); i++) {
        hashInsert(&ht_instance, name_array[i], strlen(name_array[i]) + 1, &age_array[i], sizeof(age_array[i]));
    }
#else
    char buf[4];
    for (uint32_t i = 0; i < 100; i++) {
        sprintf(buf, "%u", i);
        hashInsert(&ht_instance, buf, sizeof(buf), &i, sizeof(uint32_t));
    }
#endif
    /*hashSearch(&ht_instance, "sussi", strlen("sussi") + 1);*/
    /*hashRemove(&ht_instance, "sussi", strlen("sussi") + 1);*/
    hashRemove(&ht_instance, "messi", strlen("messi") + 1);
    hashRemove(&ht_instance, "donglu", strlen("donglu") + 1);
    printf("\n###########################################\n");
    hashTraverse(&ht_instance);
    printf("###########################################\n");
    printf("\n");

    hashDestroy(&ht_instance);

    return 0;
}




/********************************** END **********************************************/

