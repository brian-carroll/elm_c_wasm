#include "../kernel/core/core.h"
#include "test.h"

static ElmInt one = {.header = HEADER_INT, .value = 1};
static ElmInt two = {.header = HEADER_INT, .value = 2};
static ElmInt three = {.header = HEADER_INT, .value = 3};
static ElmInt four = {.header = HEADER_INT, .value = 4};
static ElmInt five = {.header = HEADER_INT, .value = 5};
static ElmInt nine = {.header = HEADER_INT, .value = 9};
static ElmInt ten = {.header = HEADER_INT, .value = 10};

void test_List_create() {
  Cons* l1 = List_create(3, (void*[]){&one, &two, &three});
  Cons* l2 = newCons(&one, newCons(&two, newCons(&three, &Nil)));

  expect_equal("List_create should match manually created list [1,2,3]", l1, l2);

  Cons* n = List_create(0, NULL);
  expect_equal("List_create should return Nil when called with no values", n, &Nil);
}

void test_List_append() {
  Cons* l1 = List_create(3, (void*[]){&one, &two, &three});
  Cons* l2 = List_create(2, (void*[]){&four, &five});

  expect_equal("[1,2,3] ++ [4,5] == [1,2,3,4,5]",
      A2(&List_append, l1, l2),
      List_create(5, (void*[]){&one, &two, &three, &four, &five}));

  expect_equal("[1,2,3] ++ [] == [1,2,3]", A2(&List_append, l1, &Nil), l1);
  expect_equal("[] ++ [4,5] == [4,5]", A2(&List_append, &Nil, l2), l2);
  expect_equal("[] ++ [] == []", A2(&List_append, &Nil, &Nil), &Nil);
}

void test_List_map2() {
  Cons* l1 = List_create(3, (void*[]){&one, &two, &three});
  Cons* l2 = List_create(2, (void*[]){&four, &five});

  expect_equal("map2 (*) [1,2,3] [4,5] == [4,10]",
      A3(&List_map2, &Basics_mul, l1, l2),
      List_create(2, (void*[]){&four, &ten}));

  expect_equal("map2 (*) [1,2,3] [1,2,3] == [1,4,9]",
      A3(&List_map2, &Basics_mul, l1, l1),
      List_create(3, (void*[]){&one, &four, &nine}));

  expect_equal("map2 (*) [4,5] [1,2,3] == [4,10]",
      A3(&List_map2, &Basics_mul, l2, l1),
      List_create(2, (void*[]){&four, &ten}));

  expect_equal("map2 (*) [1,2,3] [] == []", A3(&List_map2, &Basics_mul, l1, &Nil), &Nil);

  expect_equal("map2 (*) [] [1,2,3] == []", A3(&List_map2, &Basics_mul, &Nil, l1), &Nil);
}

void list_test() {
  if (verbose) {
    safe_printf("\n\n\n");
    safe_printf("####################################################\n");
    safe_printf("\n");
    safe_printf("List\n");
    safe_printf("------\n");
  }

  describe("test_List_create", test_List_create);
  describe("test_List_append", test_List_append);
  describe("test_List_map2", test_List_map2);
}
