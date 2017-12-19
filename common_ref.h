#ifndef __common_ref_h__
#define __common_ref_h__

#include "refmem.h"

typedef struct header header_t;

struct header
{
  int ref_count;
  function1_t destructor;
};

//Userland pointer points at the first O (The object)   HHHH_OOOOOOOOOO
//                                                           ^
//Internal pointer points at the first H (The header)   HHHH_OOOOOOOOOO
//                                                      ^
//Takes an internal pointer to an allocation and returns its userland pointer
#define allocate_object(p) (((header_t *)p) + 1)

//Takes an userland pointer to an object and returns an internal pointer
//to the start of the allocation
#define allocate_header(p) (((header_t *)p) - 1)

#endif
