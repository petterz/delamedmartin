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
void test_allocate_size() {
  char *string = "Test";
  void *object = allocate(sizeof(string), NULL);
  CU_ASSERT_TRUE(sizeof(object) == sizeof(string));
  deallocate(object);
}

void test_allocate_use_correct() {
  char *string = "Test";
  void *object = allocate(sizeof(string), NULL);
  object = "Test";
  CU_ASSERT_TRUE(strcmp((char *) object, "Test") == 0);
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

/*
Optimisering till senare, ska inte kunna använda mer minne än de allokerat
void test_allocate_use_incorrect() {
  char *Hej = "Hej";
  void *object = allocate(sizeof(Hej), NULL);
  object = "Hejj";
  CU_ASSERT_FALSE(strcmp((char *) object, "Hejj") == 0);
}
*/


//////////////////////////////TEST REF COUNT//////////////////////////////////
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

/////////////////////////////TEST DEALLOCATE//////////////////////////////////

void test_deallocate_not_valid_rc() {
  char *string = "Test";
  void *object = allocate(sizeof(string), NULL);


}


int main(void)
{
  // Initialise
  CU_initialize_registry();

  // Set up suites and tests
  CU_pSuite allocate = CU_add_suite("Testing allocate functions", NULL, NULL);
  CU_add_test(allocate, "Allocate an object", test_allocate_size);
  CU_add_test(allocate, "Use the allocated memory correct",
              test_allocate_use_correct);
  CU_add_test(allocate, "Check that the ref_count is initialized to 0", test_allocate_refcount);
  CU_add_test(allocate, "Without destructor", test_allocate_no_destructor);
  CU_add_test(allocate, "With destructor", test_allocate_destructor);

  CU_pSuite rc = CU_add_suite("Testing ref_count functions", NULL, NULL);
  CU_add_test(rc, "Check if the ref_count is correct",
              test_rc_before_first_retain);
  CU_add_test(rc, "Check if the ref_count is correct after using retain()",
              test_rc_retain);
  CU_add_test(rc, "Check if the ref_count is correct after using release()",
              test_rc_release);

  CU_pSuite free = CU_add_suite("Testing functions related to freeing memory",
                                NULL, NULL);
  CU_add_test(free, "Not free the object if the rc isn't 0",
              test_deallocate_not_valid_rc);
  // Actually run tests
  CU_basic_run_tests();

  // Tear down
  CU_cleanup_registry();
  return CU_get_error();
}
