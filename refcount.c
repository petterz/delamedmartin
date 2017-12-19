#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "refcount.h"

void retain_aux(void *obj) {
  if (obj != NULL) {
    header_t *header = allocate_header(obj);
    header->ref_count = header->ref_count + 1;
  }
}

void release_aux(void *obj) {
  if (obj != NULL) {
    header_t *header = allocate_header(obj);
    int ref_count = rc(obj);
    if (ref_count > 1) {
      header->ref_count = header->ref_count - 1;
    }
    else if (ref_count < 1) {
      printf("Can't decrement the ref_count");
      deallocate(obj);//Kalla på deallocate??
    }
    else {
      header->ref_count = header->ref_count - 1;
      deallocate(obj);
    }
  }
}

size_t rc_aux(void *obj) {
  header_t *header = allocate_header(obj);
  return header->ref_count; 
}
