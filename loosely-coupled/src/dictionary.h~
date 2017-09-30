#ifndef _Dictionary_
#define _Dictionary_
#define MAX_WORD_SIZE1 30

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct _dictionary_ *Dictionary;

Dictionary dict_load(char *filename);

void dict_save(Dictionary dict, char *filename);

Dictionary dict_add(Dictionary dict, char *word);

int dict_contains(Dictionary dict, char *word);

Dictionary dict_new(void);

void dict_destroy(Dictionary dict);

#endif
