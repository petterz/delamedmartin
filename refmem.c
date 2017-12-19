#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "refmem.h"
#include "dold.h"


void retain(void *obj) {
  retain_aux(obj);
}

void release(void *obj) {
  release_aux(obj);
}

size_t rc(void *obj) {
  return rc_aux(obj);
}

char *string_duplicate(char *s) {
  return string_duplicate_aux(s);
}

obj *allocate(size_t bytes, function1_t destructor) {
  return allocate_aux(bytes, destructor);
}

obj *allocate_array(size_t elements, size_t elem_size, function1_t destructor) {
  return allocate_array_aux(elements, elem_size, destructor);
}

void deallocate(void *obj) {
  return deallocate_aux(obj);
}

void set_cascade_limit(size_t);
size_t get_cascade_limit();
void cleanup();
void shutdown();



