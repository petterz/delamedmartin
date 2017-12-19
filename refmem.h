#include <stdlib.h>
#pragma once

typedef void obj;

/// A pointer to a function
typedef void(*function1_t)(obj *);

/// \brief Increment the objects reference counter
///
/// If the object is NULL retain does nothing
///
/// \param object A pointer to an object 
void retain(obj *object);

/// \brief Decrement the objects reference counter
///
/// When the objects reference counter decrements to 0 it will be
/// considered as garbage and will be deallocated
/// The reference count can't go lower than 0
/// If the object is NULL release does nothing
///
/// \param object A pointer to an object
void release(obj *object);

/// \brief Get the reference count of an object
///
/// \param object A pointer to an object
/// \returns: The amount of references to the object
size_t rc(obj *object);

/// \brief Allocates a memory block of size bytes 
///
/// Initializes all the bytes in the allocated memory to zero
/// The destructor function will be called right before the object is deallocated
///
/// \param bytes Size of the object to be allocated
/// \param destructor (may be NULL) A pointer to a function
/// \returns: A pointer to the allocated memory
obj *allocate(size_t bytes, function1_t destructor);

/// \brief Allocates a memory block for an array of elem_num elements with each element being elem_size bytes long
/// 
/// Initializes all the bytes in the allocated memory to zero
/// The destructor function will be called right before the array is deallocated
///
/// \param elem_num Amount of elements to be allocated
/// \param elem_size Size of the element
/// \param destructor (may be NULL) A pointer to a function
/// \returns: A pointer to the allocated memory
obj *allocate_array(size_t elem_num, size_t elem_size, function1_t destructor);

/// \brief Deallocates an objects memory block 
///
/// The objects reference count must be 0 for it to be considered garbage
/// If the reference count isn't 0, the object will not be deallocated
/// If the object is NULL deallocate does nothing
///
/// \param object A pointer to an object
void deallocate(obj *object);

/// \brief Setting an upper cascade limit on how many deallocates that are allowed at once  
///
/// \param size The limit
void set_cascade_limit(size_t size);

/// \brief Get the current cascade limit
///
/// \return: The number of the limit
size_t get_cascade_limit();

/// \brief Deallocates all allocated objects whose reference count is 0
///
/// Cleanup will proceed regardless of the cascade limit
void cleanup();

/// \brief Deallocates all allocated objects regardless of the cascade limit and the reference count
void shutdown();

/// \breif Duplicate a string
///
/// The duplicated string is allocated
///
/// \param string The string to duplicate
/// \return: A pointer to the duplicated string
char *string_duplicate(char *string);
