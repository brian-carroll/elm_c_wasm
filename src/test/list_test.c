#include "../kernel/core/core.h"
#include "test.h"

ElmInt one = {.header = HEADER_INT, .value = 1};
ElmInt two = {.header = HEADER_INT, .value = 2};
ElmInt three = {.header = HEADER_INT, .value = 3};
ElmInt four = {.header = HEADER_INT, .value = 4};
ElmInt five = {.header = HEADER_INT, .value = 5};
ElmInt nine = {.header = HEADER_INT, .value = 9};
ElmInt ten = {.header = HEADER_INT, .value = 10};

void* test_List_create() {
  Cons* l1 = List_create(3, (void* []){&one, &two, &three});
  Cons* l2 = NEW_CONS(&one, NEW_CONS(&two, NEW_CONS(&three, &Nil)));

  expect_equal("List_create should match manually created list [1,2,3]", l1, l2);

  Cons* n = List_create(0, (void* []){});
  expect_equal("List_create should return Nil when called with no values", n, &Nil);

  return NULL;
}

void* test_List_append() {
  Cons* l1 = List_create(3, (void* []){&one, &two, &three});
  Cons* l2 = List_create(2, (void* []){&four, &five});

  expect_equal("[1,2,3] ++ [4,5] == [1,2,3,4,5]",
      A2(&List_append, l1, l2),
      List_create(5, (void* []){&one, &two, &three, &four, &five}));

  expect_equal("[1,2,3] ++ [] == [1,2,3]", A2(&List_append, l1, &Nil), l1);
  expect_equal("[] ++ [4,5] == [4,5]", A2(&List_append, &Nil, l2), l2);
  expect_equal("[] ++ [] == []", A2(&List_append, &Nil, &Nil), &Nil);

  return NULL;
}

void* test_List_map2() {
  Cons* l1 = List_create(3, (void* []){&one, &two, &three});
  Cons* l2 = List_create(2, (void* []){&four, &five});

  expect_equal("map2 (*) [1,2,3] [4,5] == [4,10]",
      A3(&List_map2, &Basics_mul, l1, l2),
      List_create(2, (void* []){&four, &ten}));

  expect_equal("map2 (*) [1,2,3] [1,2,3] == [1,4,9]",
      A3(&List_map2, &Basics_mul, l1, l1),
      List_create(3, (void* []){&one, &four, &nine}));

  expect_equal("map2 (*) [4,5] [1,2,3] == [4,10]",
      A3(&List_map2, &Basics_mul, l2, l1),
      List_create(2, (void* []){&four, &ten}));

  expect_equal("map2 (*) [1,2,3] [] == []", A3(&List_map2, &Basics_mul, l1, &Nil), &Nil);

  expect_equal("map2 (*) [] [1,2,3] == []", A3(&List_map2, &Basics_mul, &Nil, l1), &Nil);

  return NULL;
}

char* list_test() {
  if (verbose) {
    printf("\n\n\n");
    printf("####################################################\n");
    printf("\n");
    printf("List\n");
    printf("------\n");
  }

  describe("test_List_create", test_List_create);
  describe("test_List_append", test_List_append);
  describe("test_List_map2", test_List_map2);

  return NULL;
}
