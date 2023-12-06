#include <stdlib.h>

#include "die.h"
#include "memory.h"
#include "value.h"

void memory_init(Memory *arr) {
  arr->len = 0;

  arr->capacity = 16;
  arr->data = malloc(arr->capacity * sizeof(InternalValue));

  if (arr->data == NULL)
    die_internal(PARSER,
                 "malloc() returned null (could not allocate data for parser)");
}

void memory_resize(Memory *arr) {
  arr->capacity *= 2;
  arr->data = realloc(arr->data, arr->capacity * sizeof(InternalValue));

  if (arr->data == NULL)
    die_internal(
        EXEC, "realloc() returned null (could not allocate data for parser)");
}

void memory_insert(Memory *arr, InternalValue *v) {
  if (arr->len == arr->capacity)
    memory_resize(arr);

  arr->data[arr->len++] = *v;
}

void memory_free(Memory *arr) { free(arr->data); }
