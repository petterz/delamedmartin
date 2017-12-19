#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <CUnit/Automated.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "common_ref.h"
#include "refmem.h"

void test_destructor(obj *test_string) {
  printf("%s", (char *)test_string);
}

//////////////////////////////TEST ALLOCATE//////////////////////////////////
//FRÅGA GUSTAF PÅ MÖTET 21/12, TESTA STORLEK PÅ ALLOKERINGEN FUNKAR EJ ALLTID 
//PGA ALLIGNMENT

/*
void test_allocate_size() {
  char *string = "Test";
  void *object = allocate(sizeof(string), NULL);
  CU_ASSERT_TRUE(sizeof(object) == sizeof(string));
}
*/

void test_allocate_use_correct() {
  char *string = "Test";
  char *object = allocate(sizeof(string), NULL);
  object = "Test";
  CU_ASSERT_TRUE(strcmp(object, "Test") == 0);
}

void test_allocate_refcount() {
  char *string = "Test";
  void *object = allocate(sizeof(string), NULL);
  header_t *header = allocate_header(object);
  CU_ASSERT_TRUE(header->ref_count == 0)
}

void test_allocate_no_destructor() {
  char *string = "Test";
  void *object = allocate(sizeof(string), NULL);
  header_t *header = allocate_header(object);
  CU_ASSERT_FALSE(header->destructor != NULL)
}

void test_allocate_destructor() {
  char *string = "Test";
  void *object = allocate(sizeof(string), test_destructor);
  header_t *header = allocate_header(object);
  CU_ASSERT_TRUE(header->destructor != NULL)
}

/////////////////////////////TEST ALLOCATE_ARRAY//////////////////////////////

//FRÅGA GUSTAF PÅ MÖTET 21/12, TESTA STORLEK PÅ ALLOKERINGEN FUNKAR EJ ALLTID 
//PGA ALLIGNMENT

/*void test_allocate_array_size_single() {
  char *string = "Test";
  void *object = allocate_array(1, sizeof(string), NULL);
  CU_ASSERT_TRUE(sizeof(object) == sizeof(string));
}

void test_allocate_array_size_multiple() {
  void *object1 = allocate_array(2, sizeof(2), NULL);
  CU_ASSERT_TRUE(sizeof(object1) == 2 * sizeof(2));
  }*/

void test_allocate_array_use_correct() {
  char *object = allocate_array(2, sizeof(int), NULL);
  object[0] = 2;
  object[1] = 10;
  CU_ASSERT_TRUE(object[0] == 2);
  CU_ASSERT_TRUE(object[1] == 10);
}

void test_allocate_array_refcount() {
  char *string = "Test";
  void *object = allocate_array(1, sizeof(string), NULL);
  header_t *header = allocate_header(object);
  CU_ASSERT_TRUE(header->ref_count == 0)
}

void test_allocate_array_no_destructor() {
  char *string = "Test";
  void *object = allocate_array(1, sizeof(string), NULL);
  header_t *header = allocate_header(object);
  CU_ASSERT_FALSE(header->destructor != NULL)
}

void test_allocate_array_destructor() {
  char *string = "Test";
  void *object = allocate_array(1, sizeof(string), test_destructor);
  header_t *header = allocate_header(object);
  CU_ASSERT_TRUE(header->destructor != NULL)
}



//////////////////////////////TEST REF_COUNT//////////////////////////////////
void test_rc_before_first_retain() {
  char *string = "Test";
  void *object = allocate(sizeof(string), NULL);
  int i = rc(object);
  CU_ASSERT_TRUE(i == 0);
}


void test_rc_retain() {
  char *string = "Test";
  void *object = allocate(sizeof(string), NULL);
  retain(object);
  int i = rc(object);
  CU_ASSERT_TRUE(i == 1);
  retain(object);
  i = rc(object);
  CU_ASSERT_TRUE(i == 2);
}

void test_rc_release() {
  char *string = "Test";
  void *object = allocate(sizeof(string), NULL);
  retain(object);
  retain(object);
  retain(object);
  int i = rc(object);
  CU_ASSERT_TRUE(i == 3);
  release(object);
  i = rc(object);
  CU_ASSERT_TRUE(i == 2);
  release(object);
  i = rc(object);
  CU_ASSERT_TRUE(i == 1);
}

void test_rc_release_rc0() {
  char *string = "Test";
  void *object = allocate(sizeof(string), NULL);
  retain(object);
  retain(object);
  retain(object);
  int i = rc(object);
  CU_ASSERT_TRUE(i == 3);
  release(object);
  i = rc(object);
  CU_ASSERT_TRUE(i == 2);
  release(object);
  i = rc(object);
  CU_ASSERT_TRUE(i == 1);
}


int main(void)
{
  // Initialise
  CU_initialize_registry();

  // Set up suites and tests
  CU_pSuite allocate = CU_add_suite("Testing allocate functions", NULL, NULL);
  //CU_add_test(allocate, "Allocate an object", test_allocate_size);
  CU_add_test(allocate, "Use the allocated memory correct",
              test_allocate_use_correct);
  CU_add_test(allocate, "Check that the ref_count is initialized to 0",
              test_allocate_refcount);
  CU_add_test(allocate, "Without destructor", test_allocate_no_destructor);
  CU_add_test(allocate, "With destructor", test_allocate_destructor);

  // CU_add_test(allocate, "Allocate an array with a single element",
  //            test_allocate_array_size_single);
  //CU_add_test(allocate, "Allocate an array with multiple elements",
  //            test_allocate_array_size_multiple);
  CU_add_test(allocate, "Use the allocated memory correct",
              test_allocate_array_use_correct);
  CU_add_test(allocate, "Check that the ref_count is initialized to 0",
              test_allocate_refcount);
  CU_add_test(allocate, "Without destructor",
              test_allocate_array_no_destructor);
  CU_add_test(allocate, "With destructor",
              test_allocate_array_destructor);

  
  CU_pSuite rc = CU_add_suite("Testing ref_count functions", NULL, NULL);
  CU_add_test(rc, "Check if the ref_count is correct",
              test_rc_before_first_retain);
  CU_add_test(rc, "Check if the ref_count is correct after using retain()",
              test_rc_retain);
  CU_add_test(rc, "Check if the ref_count is correct after using release()",
              test_rc_release);

  // Actually run tests
  CU_basic_run_tests();

  // Tear down
  CU_cleanup_registry();
  return CU_get_error();
}
