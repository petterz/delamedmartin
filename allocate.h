#ifndef __allocate_h__
#define __allocate_h__

#include <stdlib.h>
#include <stdbool.h>
#include "common_ref.h"

char *string_duplicate_aux(char *s);
obj *allocate_aux(size_t bytes, function1_t destructor);
obj *allocate_array_aux(size_t elements, size_t elem_size, function1_t destructor);
void free_allocation(void *obj);
bool check_destructor(header_t *header);
function1_t get_destructor(header_t *header);
void deallocate_aux(void *obj);

#endif



