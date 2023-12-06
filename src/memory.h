#ifndef MEMORY_H
#define MEMORY_H

#include "value.h"

typedef struct {
  unsigned int len;
  unsigned int capacity;
  InternalValue *data;
} Memory;

void memory_init(Memory *arr);
void memory_resize(Memory *arr);
void memory_insert(Memory *arr, InternalValue *v);
void memory_free(Memory *arr);

#endif
