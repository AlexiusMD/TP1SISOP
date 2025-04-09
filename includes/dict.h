#ifndef DICT_H
#define DICT_H

typedef struct Dict {
    char* key;
    int value;
} Dict;

typedef struct DictList {
    Dict* items;
    int capacity;
    int size;
} DictList;

bool dictListInit(int initial_capacity);
bool dictListAdd(DictList* dictList, const char* key, int value);
bool dictListFree(DictList* dictList);

#endif