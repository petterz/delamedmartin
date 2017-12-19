#ifndef __refcount_h__
#define __refcount_h__

#include <stdlib.h>
#include <stdbool.h>
#include "common_ref.h"


void retain_aux(void *obj);
void release_aux(void *obj);
size_t rc_aux(void *obj);

#endif
