#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "allocate.h"

static int size_ref_count = 8;
static int starting_ref_count = 0;

char *string_duplicate_aux(char *s) {
  char *string = allocate(strlen(s) + 1, NULL);
  if (string == NULL) {
    return NULL;
  }
  retain(string);
  strcpy(string, s);
  return string;
}

obj *allocate_aux(size_t bytes, function1_t destructor) {
  size_t size = bytes + sizeof(destructor) + size_ref_count;
  header_t *meta_data = malloc(size);
  memset(meta_data, 0, size);

  meta_data->ref_count = starting_ref_count;
  meta_data->destructor = destructor;
  
  return (obj *)allocate_object(meta_data); //Användaren behöver bara objektet
}

obj *allocate_array_aux(size_t elements, size_t elem_size, function1_t destructor) {
  size_t size = elements*elem_size + sizeof(destructor) + size_ref_count;
  header_t *meta_data = calloc(1, size);

  meta_data->ref_count = starting_ref_count;
  meta_data->destructor = destructor;

  return (obj *)allocate_object(meta_data);
}

/// \brief Free an allocated memory block
///
/// \param obj A pointer to the memory block
void free_allocation(void *obj) {
  header_t *header = allocate_header(obj);
  free(header);
}

/// \brief Check if an object has a destructor
///
/// \param header A pointer to an objects header
/// \returns: true if the object has a destructor, else false
bool check_destructor(header_t *header) {
  if (header->destructor == NULL) {
    return false;
  }
  return true;
}

/// \brief Get an objects destructor
///
/// \param header A pointer to an objects header
/// \returns The destructor or NULL if it doesn't exist
function1_t get_destructor(header_t *header) {
  if (check_destructor(header)) {
    return header->destructor;
  }
  return NULL;
}


void deallocate_aux(void *obj) {
  if (obj != NULL && rc(obj) == 0) {
    header_t *header = allocate_header(obj);
    if (!check_destructor(header)) {
      free_allocation(obj);
    } else {
      function1_t destr = get_destructor(header);
      destr(obj);
      free_allocation(obj);
    }
  } 
}


