
#include <stdlib.h>
#include <string.h>

#include "../kernel/core/core.h"
#include "test.h"

char* test_records() {
  if (verbose) {
    printf("\n");
    printf("## Record access & update\n");
    printf("\n");

    printf(
        "type alias Record1 =\n"
        "    { someField: Int\n"
        "    , otherField: String\n"
        "    }\n"
        "\n"
        "-- differently shaped record type with 'someField' in a different position\n"
        "type alias Record2 =\n"
        "    { things: Int\n"
        "    , someField: Int\n"
        "    , stuff: Float\n"
        "    }\n"
        "\n"
        "r1 = Record1 123 \"hello\"\n"
        "r2 = Record2 456 321 1.0\n"
        "\n"
        "int1 = .someField r1  -- 123\n"
        "int2 = .someField r2  -- 321\n"
        "\n"
        "r3 = { r2\n"
        "        | things = 111\n"
        "        , stuff = 3.14\n"
        "        }\n"
        "\n");
  }

  // Elm compiler transforms field names to numbers
  u32 things = 12;
  u32 someField = 23;
  u32 otherField = 42;
  u32 stuff = 71;

  u8 mem_fsRecord1[sizeof(FieldGroup) + 2 * sizeof(u32)];
  u8 mem_fsRecord2[sizeof(FieldGroup) + 3 * sizeof(u32)];

  FieldGroup* fgRecord1 = (FieldGroup*)mem_fsRecord1;
  FieldGroup* fgRecord2 = (FieldGroup*)mem_fsRecord2;

  fgRecord1->size = 2;
  fgRecord1->fields[0] = someField;
  fgRecord1->fields[1] = otherField;

  fgRecord2->size = 3;
  fgRecord2->fields[0] = things;
  fgRecord2->fields[1] = someField;
  fgRecord2->fields[2] = stuff;

  u8 mem_r1[sizeof(Record) + 2 * sizeof(void*)];
  u8 mem_r2[sizeof(Record) + 3 * sizeof(void*)];

  Record* r1 = (Record*)mem_r1;
  Record* r2 = (Record*)mem_r2;

  r1->header = (Header)HEADER_RECORD(2);
  r1->fieldgroup = fgRecord1;
  r1->values[0] = NEW_ELM_INT(123);
  r1->values[1] = NEW_ELM_STRING(5, "hello");

  r2->header = (Header)HEADER_RECORD(3);
  r2->fieldgroup = fgRecord2;
  r2->values[0] = NEW_ELM_INT(456);
  r2->values[1] = NEW_ELM_INT(321);
  r2->values[2] = NEW_ELM_FLOAT(1.0);

  // The actual accessor function
  u8 mem_accessor[sizeof(Closure) + sizeof(void*)];
  Closure* access_someField = (Closure*)mem_accessor;
  access_someField->header = (Header)HEADER_CLOSURE(1);
  access_someField->max_values = 2;
  access_someField->n_values = 1;
  access_someField->evaluator = &Utils_access_eval;
  access_someField->values[0] = (void*)(size_t)someField;

  ElmInt* int1 = A1(access_someField, r1);
  ElmInt* int2 = A1(access_someField, r2);

  /*
      r3 = { r2
             | things = 111
             , stuff = 3.14
           }
  */
  ElmInt* updated_thing = NEW_ELM_INT(111);
  ElmFloat* updated_stuff = NEW_ELM_FLOAT(3.14);
  Record* r3 = Utils_update(
      r2, 2, (u32[]){things, stuff}, (void*[]){updated_thing, updated_stuff});

  if (verbose) {
    printf(
        "r1: addr=%zx val=%s\n", (size_t)r1, hex(r1, sizeof(Record) + 2 * sizeof(void*)));
    printf(
        "r2: addr=%zx val=%s\n", (size_t)r2, hex(r2, sizeof(Record) + 3 * sizeof(void*)));
    printf("fieldgroup Record1: addr=%zx val=%s\n",
        (size_t)fgRecord1,
        hex(fgRecord1, sizeof(FieldGroup) + 2 * sizeof(u32)));
    printf("fieldgroup for r2: addr=%zx val=%s\n",
        (size_t)fgRecord2,
        hex(fgRecord2, sizeof(FieldGroup) + 3 * sizeof(u32)));
    printf("Closure access_someField = %s\n",
        hex(access_someField, sizeof(Closure) + sizeof(void*)));
    printf(
        "r3: addr=%zx val=%s\n", (size_t)r3, hex(r3, sizeof(Record) + 3 * sizeof(void*)));
  }

  mu_assert("Record accessor should return 123 for r1", int1->value == 123);
  mu_assert("Record accessor should return 321 for r2", int2->value == 321);

  mu_assert("Updated record r3 should have same fieldgroup as r2",
      r3->fieldgroup == r2->fieldgroup);
  mu_assert("Updated record r3 should have same header as r2",
      memcmp(r3, r2, sizeof(Header)) == 0);
  mu_assert("Updated record should have 3 correct field values",
      r3->values[0] == updated_thing && r3->values[1] == r2->values[1] &&
          r3->values[2] == updated_stuff);

  return NULL;
}

// This is code I could generate from the Elm compiler. I could write better by hand.
// The intermediate ElmInt allocated on heap is hard to eliminate in generated code.
// Would be nice to evaluate entire expression on raw ints, and only then box it in ElmInt
// Would require Elm compiler optimizer to identify Int-only subexpressions (also
// Float-only) so that the code generator would know what to do.
void* eval_user_project_closure(void* args[]) {
  ElmInt* outerScopeValue = args[0];
  ElmInt* arg1 = args[1];
  ElmInt* arg2 = args[2];

  ElmInt* tmp = A2(&Basics_add, arg2, outerScopeValue);
  return A2(&Basics_add, arg1, tmp);
}

/*
    outerScopeValue =
        1

    closure arg1 arg2 =
        outerScopeValue + arg1 + arg2

    curried =
        closure 2

    answer =
        curried 3
*/

char* test_apply(void) {
  if (verbose) {
    printf("\n\n");
    printf("## Apply\n");
    printf("\n");
  }

  Closure user_project_closure = (Closure){.header = HEADER_CLOSURE(0),
      .evaluator = &eval_user_project_closure,
      .max_values = 3,
      .n_values = 0};

  ElmInt outerScopeValue = (ElmInt){.header = HEADER_INT, .value = 1};
  ElmInt two = (ElmInt){.header = HEADER_INT, .value = 2};
  ElmInt three = (ElmInt){.header = HEADER_INT, .value = 3};

  Closure* closure = A1(&user_project_closure, &outerScopeValue);
  Closure* curried = A1(closure, &two);
  ElmInt* answer = A1(curried, &three);

  if (verbose) {
    printf(
        "Example Elm function exercising most code paths in `apply`:\n"
        "    outerScopeValue : Int\n"
        "    outerScopeValue =\n"
        "        1\n"
        "\n"
        "    closure arg1 arg2 =\n"
        "        outerScopeValue + arg1 + arg2\n"
        "\n"
        "    curried = \n"
        "        closure 2\n"
        "\n"
        "    answer =\n"
        "        curried 3\n"
        "\n");

    printf("outerScopeValue addr=%s ctor=%d value=%d, hex=%s\n",
        hex_ptr(&outerScopeValue),
        (int)outerScopeValue.header.tag,
        outerScopeValue.value,
        hex(&outerScopeValue, sizeof(ElmInt)));

    printf("two addr=%s ctor=%d value=%d, hex=%s\n",
        hex_ptr(&two),
        (int)two.header.tag,
        two.value,
        hex(&two, sizeof(ElmInt)));

    printf("three addr=%s ctor=%d value=%d, hex=%s\n",
        hex_ptr(&three),
        (int)three.header.tag,
        three.value,
        hex(&three, sizeof(ElmInt)));

    printf("closure addr=%s n_values=%d max_values=%d, hex=%s\n",
        hex_ptr(closure),
        (int)closure->n_values,
        (int)closure->max_values,
        hex(closure, sizeof(Closure) + closure->n_values * sizeof(void*)));

    printf("curried addr=%s n_values=%d max_values=%d, hex=%s\n",
        hex_ptr(curried),
        (int)curried->n_values,
        (int)curried->max_values,
        hex(curried, sizeof(Closure) + curried->n_values * sizeof(void*)));

    printf("answer addr=%s ctor=%d value=%d, hex=%s\n",
        hex_ptr(answer),
        (int)answer->header.tag,
        answer->value,
        hex(answer, sizeof(ElmInt)));
    printf("\n");
  }

  ElmInt expected_answer = (ElmInt){.header = HEADER_INT, .value = 6};
  mu_assert("Utils_apply: Example function call should return ElmInt 6",
      memcmp(answer, &expected_answer, sizeof(ElmInt)) == 0);
  return NULL;
}

char* test_eq(void) {
  if (verbose) {
    printf("\n\n");
    printf("## Equality\n");
    printf("\n");
  }

  mu_assert("Expect: () == ()", A2(&Utils_equal, &Unit, &Unit) == &True);

  mu_assert("Expect: True == True", A2(&Utils_equal, &True, &True) == &True);
  mu_assert("Expect: False == False", A2(&Utils_equal, &False, &False) == &True);
  mu_assert("Expect: True /= False", A2(&Utils_equal, &True, &False) == &False);
  mu_assert("Expect: False /= True", A2(&Utils_equal, &False, &True) == &False);

  ElmInt two = (ElmInt){.header = HEADER_INT, .value = 2};
  ElmInt three = (ElmInt){.header = HEADER_INT, .value = 3};

  mu_assert("Expect: 2 == 2", A2(&Utils_equal, &two, &two) == &True);
  mu_assert("Expect: 2 /= 3", A2(&Utils_equal, &two, &three) == &False);

  ElmFloat* f = NEW_ELM_FLOAT(123.456);
  ElmFloat* f1 = NEW_ELM_FLOAT(123.456);
  ElmFloat* f2 = NEW_ELM_FLOAT(2.0);
  mu_assert("Expect: 123.456 == 123.456 (by reference)", A2(&Utils_equal, f, f) == &True);
  mu_assert("Expect: 123.456 == 123.456 (by value)", A2(&Utils_equal, f, f1) == &True);
  mu_assert("Expect: 123.456 /= 2.0", A2(&Utils_equal, f, f2) == &False);

  ElmChar a1 = (ElmChar){.header = HEADER_CHAR, .value = 'A'};
  ElmChar a2 = (ElmChar){.header = HEADER_CHAR, .value = 'A'};
  ElmChar b = (ElmChar){.header = HEADER_CHAR, .value = 'B'};
  mu_assert("Expect: 'A' == 'A', by reference", A2(&Utils_equal, &a1, &a1) == &True);
  mu_assert("Expect: 'A' == 'A', by value", A2(&Utils_equal, &a1, &a2) == &True);
  mu_assert("Expect: 'A' /= 'B'", A2(&Utils_equal, &a1, &b) == &False);

  if (verbose) {
    printf("\n");
    printf("Different types: compiler will reject, but `==` can give False anyway\n");
  }
  mu_assert("Expect: True /= 3", A2(&Utils_equal, &True, &three) == &False);

  ElmString* hello1 = NEW_ELM_STRING(5, "hello");
  ElmString* hello2 = NEW_ELM_STRING(5, "hello");
  ElmString* hello_ = NEW_ELM_STRING(6, "hello_");
  ElmString* world = NEW_ELM_STRING(5, "world");

  if (verbose) {
    printf("\nString equality\n");
    printf("hello1 str=\"%s\" hex=%s\n",
        hello1->bytes,
        hex(hello1, hello1->header.size * 4));
    printf("hello2 str=\"%s\" hex=%s\n",
        hello2->bytes,
        hex(hello2, hello2->header.size * 4));
    printf("hello_ str=\"%s\" hex=%s\n",
        hello_->bytes,
        hex(hello_, hello_->header.size * 4));
    printf("world str=\"%s\" hex=%s\n", world->bytes, hex(world, world->header.size * 4));
  }

  mu_assert("Expect: \"hello\" == \"hello\" (by reference)",
      A2(&Utils_equal, hello1, hello1) == &True);
  mu_assert("Expect: \"hello\" == \"hello\" (by value)",
      A2(&Utils_equal, hello1, hello2) == &True);
  mu_assert("Expect: \"hello\" /= \"world\"", A2(&Utils_equal, hello1, world) == &False);
  mu_assert(
      "Expect: \"hello\" /= \"hello_\"", A2(&Utils_equal, hello1, hello_) == &False);
  mu_assert(
      "Expect: \"hello_\" /= \"hello\"", A2(&Utils_equal, hello_, hello1) == &False);

  if (verbose) printf("\nList equality\n");
  Cons* cons2 = NEW_CONS(&two, &Nil);
  Cons* cons2a = NEW_CONS(&two, &Nil);
  Cons* cons3 = NEW_CONS(&three, &Nil);
  Cons* cons23 = NEW_CONS(&two, cons3);
  Cons* cons23a = NEW_CONS(&two, cons3);
  Cons* cons32 = NEW_CONS(&three, cons2);
  Cons* cons22 = NEW_CONS(&two, cons2);

  mu_assert("Expect: [] == []", A2(&Utils_equal, &Nil, &Nil) == &True);
  mu_assert("Expect: [] /= [2]", A2(&Utils_equal, &Nil, cons2) == &False);
  mu_assert("Expect: [2] /= []", A2(&Utils_equal, cons2, &Nil) == &False);
  mu_assert("Expect: [2] == [2] (by ref)", A2(&Utils_equal, cons2, cons2) == &True);
  mu_assert("Expect: [2] == [2] (by value)", A2(&Utils_equal, cons2, cons2a) == &True);
  mu_assert("Expect: [2] /= [3]", A2(&Utils_equal, cons2, cons3) == &False);
  mu_assert("Expect: [2] /= [2,3]", A2(&Utils_equal, cons2, cons23) == &False);
  mu_assert("Expect: [2,3] == [2,3] (by ref)", A2(&Utils_equal, cons23, cons23) == &True);
  mu_assert(
      "Expect: [2,3] == [2,3] (by value)", A2(&Utils_equal, cons23, cons23a) == &True);
  mu_assert("Expect: [3,2] /= [2,2]", A2(&Utils_equal, cons32, cons22) == &False);
  mu_assert("Expect: [2,3] /= [2,2]", A2(&Utils_equal, cons23, cons22) == &False);

  if (verbose) printf("\nTuple equality\n");
  Tuple2* tuple23 = NEW_TUPLE2(&two, &three);
  Tuple2* tuple23a = NEW_TUPLE2(&two, &three);
  Tuple2* tuple32 = NEW_TUPLE2(&three, &two);
  Tuple2* tuple22 = NEW_TUPLE2(&two, &two);

  mu_assert(
      "Expect: (2,3) == (2,3) (by ref)", A2(&Utils_equal, tuple23, tuple23) == &True);
  mu_assert(
      "Expect: (2,3) == (2,3) (by value)", A2(&Utils_equal, tuple23, tuple23a) == &True);
  mu_assert("Expect: (3,2) /= (2,2)", A2(&Utils_equal, tuple32, tuple22) == &False);
  mu_assert("Expect: (2,3) /= (2,2)", A2(&Utils_equal, tuple23, tuple22) == &False);

  ElmInt one = (ElmInt){.header = HEADER_INT, .value = 1};
  Tuple3* tuple123 = NEW_TUPLE3(&one, &two, &three);
  Tuple3* tuple123a = NEW_TUPLE3(&one, &two, &three);
  Tuple3* tuple111 = NEW_TUPLE3(&one, &one, &one);
  Tuple3* tuple211 = NEW_TUPLE3(&two, &one, &one);
  Tuple3* tuple121 = NEW_TUPLE3(&one, &two, &one);
  Tuple3* tuple112 = NEW_TUPLE3(&one, &one, &two);

  mu_assert("Expect: (1,2,3) == (1,2,3) (by ref)",
      A2(&Utils_equal, tuple123, tuple123) == &True);
  mu_assert("Expect: (1,2,3) == (1,2,3) (by value)",
      A2(&Utils_equal, tuple123, tuple123a) == &True);
  mu_assert("Expect: (1,1,1) /= (2,1,1)", A2(&Utils_equal, tuple111, tuple211) == &False);
  mu_assert("Expect: (1,1,1) /= (1,2,1)", A2(&Utils_equal, tuple111, tuple121) == &False);
  mu_assert("Expect: (1,1,1) /= (1,1,2)", A2(&Utils_equal, tuple111, tuple112) == &False);

  u32 big_list_size = 123;
  if (verbose) printf("\nLong list equality (recursive, %d elements)\n", big_list_size);
  Cons* bigList1 = NEW_CONS(&one, &Nil);
  Cons* bigList2 = NEW_CONS(&one, &Nil);
  Cons* bigList3 = cons2;
  for (u32 i = 0; i < big_list_size; ++i) {
    bigList1 = NEW_CONS(&one, bigList1);
    bigList2 = NEW_CONS(&one, bigList2);
    bigList3 = NEW_CONS(&one, bigList3);
  }
  mu_assert("Expect: [1,1,1, ... ,1] == [1,1,1, ... ,1]",
      A2(&Utils_equal, bigList1, bigList2) == &True);
  mu_assert("Expect: [1,1,1, ... ,1] /= [1,1,1, ... ,2]",
      A2(&Utils_equal, bigList1, bigList3) == &False);

  if (verbose) printf("\nCustom type equality\n");
  u8 mem_1_1A[sizeof(Custom) + 2 * sizeof(void*)];
  Custom* custom_1_1A = (Custom*)mem_1_1A;

  custom_1_1A->header = (Header)HEADER_CUSTOM(2);
  custom_1_1A->ctor = 1;
  custom_1_1A->values[0] = &one;
  custom_1_1A->values[1] = &a1;

  Custom* custom_1_1A_clone = Utils_clone(custom_1_1A);

  Custom* custom_2_1A = Utils_clone(custom_1_1A);
  custom_2_1A->ctor = 2;

  Custom* custom_1_2A = Utils_clone(custom_1_1A);
  custom_1_2A->values[0] = &two;

  Custom* custom_1_1B = Utils_clone(custom_1_1A);
  custom_1_1B->values[1] = &b;

  mu_assert("Expect: Ctor1 1 'A' == Ctor1 1 'A' (ref)",
      A2(&Utils_equal, custom_1_1A, custom_1_1A) == &True);
  mu_assert("Expect: Ctor1 1 'A' == Ctor1 1 'A' (value)",
      A2(&Utils_equal, custom_1_1A, custom_1_1A_clone) == &True);
  mu_assert("Expect: Ctor1 1 'A' /= Ctor2 1 'A'",
      A2(&Utils_equal, custom_1_1A, custom_2_1A) == &False);
  mu_assert("Expect: Ctor1 1 'A' /= Ctor1 2 'A'",
      A2(&Utils_equal, custom_1_1A, custom_1_2A) == &False);
  mu_assert("Expect: Ctor1 1 'A' /= Ctor1 1 'B'",
      A2(&Utils_equal, custom_1_1A, custom_1_1B) == &False);

  if (verbose) printf("\nRecord equality\n");
  u8 mem_fs[sizeof(FieldGroup) + 2 * sizeof(void*)];
  FieldGroup* fg = (FieldGroup*)mem_fs;
  fg->size = 2;
  fg->fields[0] = 123;
  fg->fields[1] = 321;

  u8 mem_rec12[sizeof(Record) + 2 * sizeof(void*)];
  Record* rec12 = (Record*)mem_rec12;
  rec12->header = (Header)HEADER_RECORD(2);
  rec12->fieldgroup = fg;
  rec12->values[0] = &one;
  rec12->values[1] = &two;

  Record* rec12a = Utils_clone(rec12);
  Record* rec22 = Utils_clone(rec12);
  rec22->values[0] = &two;
  Record* rec13 = Utils_clone(rec12);
  rec13->values[1] = &three;

  if (verbose) {
    printf("rec12  = %s\n", hex(rec12, sizeof(mem_rec12)));
    printf("rec12a = %s\n", hex(rec12a, sizeof(mem_rec12)));
    printf("rec22  = %s\n", hex(rec22, sizeof(mem_rec12)));
    printf("rec13  = %s\n", hex(rec13, sizeof(mem_rec12)));
  }
  mu_assert(
      "Expect: {a=1, b=2} == {a=1, b=2} (ref)", A2(&Utils_equal, rec12, rec12) == &True);
  mu_assert("Expect: {a=1, b=2} == {a=1, b=2} (value)",
      A2(&Utils_equal, rec12, rec12a) == &True);
  mu_assert("Expect: {a=1, b=2} /= {a=2, b=2}", A2(&Utils_equal, rec12, rec22) == &False);
  mu_assert("Expect: {a=1, b=2} /= {a=1, b=3}", A2(&Utils_equal, rec12, rec13) == &False);

  return NULL;
}

char* test_compare() {
  if (verbose) {
    printf("\n\n");
    printf("## Compare\n");
    printf("\nElmInt\n");
  }
  ElmInt* i123 = NEW_ELM_INT(123);
  ElmInt* i123a = NEW_ELM_INT(123);
  ElmInt* i456 = NEW_ELM_INT(456);
  mu_assert("compare: 123 < 456", A2(&Utils_compare, i123, i456) == &g_elm_core_Basics_LT);
  mu_assert("compare: 456 > 123", A2(&Utils_compare, i456, i123) == &g_elm_core_Basics_GT);
  mu_assert("compare: 123 == 123 (ref)", A2(&Utils_compare, i123, i123) == &g_elm_core_Basics_EQ);
  mu_assert("compare: 123 == 123 (value)", A2(&Utils_compare, i123, i123a) == &g_elm_core_Basics_EQ);

  if (verbose) printf("\nElmFloat\n");
  ElmFloat* f1 = NEW_ELM_FLOAT(123.456);
  ElmFloat* f1a = NEW_ELM_FLOAT(123.456);
  ElmFloat* f2 = NEW_ELM_FLOAT(456.789);
  mu_assert("compare: 123.456 < 456.789", A2(&Utils_compare, f1, f2) == &g_elm_core_Basics_LT);
  mu_assert("compare: 456.789 > 123.456", A2(&Utils_compare, f2, f1) == &g_elm_core_Basics_GT);
  mu_assert("compare: 123.456 == 123.456 (ref)", A2(&Utils_compare, f1, f1) == &g_elm_core_Basics_EQ);
  mu_assert(
      "compare: 123.456 == 123.456 (value)", A2(&Utils_compare, f1, f1a) == &g_elm_core_Basics_EQ);

  if (verbose) printf("\nElmChar\n");
  ElmChar a1 = (ElmChar){.header = HEADER_CHAR, .value = 'A'};
  ElmChar a2 = (ElmChar){.header = HEADER_CHAR, .value = 'A'};
  ElmChar b = (ElmChar){.header = HEADER_CHAR, .value = 'B'};
  mu_assert(
      "Expect: 'A' == 'A', by reference", A2(&Utils_compare, &a1, &a1) == &g_elm_core_Basics_EQ);
  mu_assert("Expect: 'A' == 'A', by value", A2(&Utils_compare, &a1, &a2) == &g_elm_core_Basics_EQ);
  mu_assert("Expect: 'A' < 'B'", A2(&Utils_compare, &a1, &b) == &g_elm_core_Basics_LT);
  mu_assert("Expect: 'B' > 'A'", A2(&Utils_compare, &b, &a1) == &g_elm_core_Basics_GT);

  if (verbose) printf("\nTuple2\n");
  ElmInt one = (ElmInt){.header = HEADER_INT, .value = 1};
  ElmInt two = (ElmInt){.header = HEADER_INT, .value = 2};
  ElmInt three = (ElmInt){.header = HEADER_INT, .value = 3};

  Tuple2* tuple23 = NEW_TUPLE2(&two, &three);
  Tuple2* tuple23a = NEW_TUPLE2(&two, &three);
  Tuple2* tuple32 = NEW_TUPLE2(&three, &two);
  Tuple2* tuple22 = NEW_TUPLE2(&two, &two);

  mu_assert("Expect: (2,3) == (2,3) (by ref)",
      A2(&Utils_compare, tuple23, tuple23) == &g_elm_core_Basics_EQ);
  mu_assert("Expect: (2,3) == (2,3) (by value)",
      A2(&Utils_compare, tuple23, tuple23a) == &g_elm_core_Basics_EQ);
  mu_assert("Expect: (3,2) > (2,2)", A2(&Utils_compare, tuple32, tuple22) == &g_elm_core_Basics_GT);
  mu_assert("Expect: (2,3) > (2,2)", A2(&Utils_compare, tuple23, tuple22) == &g_elm_core_Basics_GT);
  mu_assert("Expect: (2,2) < (3,2)", A2(&Utils_compare, tuple22, tuple32) == &g_elm_core_Basics_LT);
  mu_assert("Expect: (2,2) < (2,3)", A2(&Utils_compare, tuple22, tuple23) == &g_elm_core_Basics_LT);

  if (verbose) printf("\nTuple3\n");
  Tuple3* tuple123 = NEW_TUPLE3(&one, &two, &three);
  Tuple3* tuple123a = NEW_TUPLE3(&one, &two, &three);
  Tuple3* tuple111 = NEW_TUPLE3(&one, &one, &one);
  Tuple3* tuple211 = NEW_TUPLE3(&two, &one, &one);
  Tuple3* tuple121 = NEW_TUPLE3(&one, &two, &one);
  Tuple3* tuple112 = NEW_TUPLE3(&one, &one, &two);

  mu_assert("Expect: (1,2,3) == (1,2,3) (by ref)",
      A2(&Utils_compare, tuple123, tuple123) == &g_elm_core_Basics_EQ);
  mu_assert("Expect: (1,2,3) == (1,2,3) (by value)",
      A2(&Utils_compare, tuple123, tuple123a) == &g_elm_core_Basics_EQ);
  mu_assert(
      "Expect: (1,1,1) < (2,1,1)", A2(&Utils_compare, tuple111, tuple211) == &g_elm_core_Basics_LT);
  mu_assert(
      "Expect: (1,1,1) < (1,2,1)", A2(&Utils_compare, tuple111, tuple121) == &g_elm_core_Basics_LT);
  mu_assert(
      "Expect: (1,1,1) < (1,1,2)", A2(&Utils_compare, tuple111, tuple112) == &g_elm_core_Basics_LT);
  mu_assert(
      "Expect: (2,1,1) > (1,1,1)", A2(&Utils_compare, tuple211, tuple111) == &g_elm_core_Basics_GT);
  mu_assert(
      "Expect: (1,2,1) > (1,1,1)", A2(&Utils_compare, tuple121, tuple111) == &g_elm_core_Basics_GT);
  mu_assert(
      "Expect: (1,1,2) > (1,1,1)", A2(&Utils_compare, tuple112, tuple111) == &g_elm_core_Basics_GT);

  if (verbose) printf("\nList\n");
  Cons* cons2 = NEW_CONS(&two, &Nil);
  Cons* cons2a = NEW_CONS(&two, &Nil);
  Cons* cons3 = NEW_CONS(&three, &Nil);
  Cons* cons23 = NEW_CONS(&two, cons3);
  Cons* cons23a = NEW_CONS(&two, cons3);
  Cons* cons32 = NEW_CONS(&three, cons2);
  Cons* cons22 = NEW_CONS(&two, cons2);

  mu_assert("Expect: [] == []", A2(&Utils_compare, &Nil, &Nil) == &g_elm_core_Basics_EQ);
  mu_assert("Expect: [] < [2]", A2(&Utils_compare, &Nil, cons2) == &g_elm_core_Basics_LT);
  mu_assert("Expect: [2] > []", A2(&Utils_compare, cons2, &Nil) == &g_elm_core_Basics_GT);
  mu_assert("Expect: [2] == [2] (by ref)", A2(&Utils_compare, cons2, cons2) == &g_elm_core_Basics_EQ);
  mu_assert(
      "Expect: [2] == [2] (by value)", A2(&Utils_compare, cons2, cons2a) == &g_elm_core_Basics_EQ);
  mu_assert("Expect: [2] < [3]", A2(&Utils_compare, cons2, cons3) == &g_elm_core_Basics_LT);
  mu_assert("Expect: [2] < [2,3]", A2(&Utils_compare, cons2, cons23) == &g_elm_core_Basics_LT);
  mu_assert("Expect: [3] > [2]", A2(&Utils_compare, cons3, cons2) == &g_elm_core_Basics_GT);
  mu_assert("Expect: [2,3] > [2]", A2(&Utils_compare, cons23, cons2) == &g_elm_core_Basics_GT);
  mu_assert(
      "Expect: [2,3] == [2,3] (by ref)", A2(&Utils_compare, cons23, cons23) == &g_elm_core_Basics_EQ);
  mu_assert("Expect: [2,3] == [2,3] (by value)",
      A2(&Utils_compare, cons23, cons23a) == &g_elm_core_Basics_EQ);
  mu_assert("Expect: [3,2] > [2,2]", A2(&Utils_compare, cons32, cons22) == &g_elm_core_Basics_GT);
  mu_assert("Expect: [2,3] > [2,2]", A2(&Utils_compare, cons23, cons22) == &g_elm_core_Basics_GT);
  mu_assert("Expect: [2,2] < [3,2]", A2(&Utils_compare, cons22, cons32) == &g_elm_core_Basics_LT);
  mu_assert("Expect: [2,2] < [2,3]", A2(&Utils_compare, cons22, cons23) == &g_elm_core_Basics_LT);

  if (verbose) printf("\nLong list (recursive)\n");
  Cons* bigList1 = NEW_CONS(&one, &Nil);
  Cons* bigList2 = NEW_CONS(&one, &Nil);
  Cons* bigList3 = cons2;
  for (u32 i = 0; i < 123; ++i) {
    bigList1 = NEW_CONS(&one, bigList1);
    bigList2 = NEW_CONS(&one, bigList2);
    bigList3 = NEW_CONS(&one, bigList3);
  }
  mu_assert("Expect: [1,1,1, ... ,1] == [1,1,1, ... ,1]",
      A2(&Utils_compare, bigList1, bigList2) == &g_elm_core_Basics_EQ);
  mu_assert("Expect: [1,1,1, ... ,1] < [1,1,1, ... ,2]",
      A2(&Utils_compare, bigList1, bigList3) == &g_elm_core_Basics_LT);
  mu_assert("Expect: [1,1,1, ... ,2] > [1,1,1, ... ,1]",
      A2(&Utils_compare, bigList3, bigList1) == &g_elm_core_Basics_GT);

  if (verbose) printf("\nTest <, <=, >, >=\n");
  mu_assert("Utils_lt: 123 < 456 == True", A2(&Utils_lt, i123, i456) == &True);
  mu_assert("Utils_lt: 456 < 123 == False", A2(&Utils_lt, i456, i123) == &False);
  mu_assert("Utils_lt: 123 < 123 == False", A2(&Utils_lt, i123, i123a) == &False);

  mu_assert("Utils_le: 123 <= 456 == True", A2(&Utils_le, i123, i456) == &True);
  mu_assert("Utils_le: 456 <= 123 == False", A2(&Utils_le, i456, i123) == &False);
  mu_assert("Utils_le: 123 <= 123 == True", A2(&Utils_le, i123, i123a) == &True);

  mu_assert("Utils_gt: 123 > 456 == False", A2(&Utils_gt, i123, i456) == &False);
  mu_assert("Utils_gt: 456 > 123 == True", A2(&Utils_gt, i456, i123) == &True);
  mu_assert("Utils_gt: 123 > 123 == False", A2(&Utils_gt, i123, i123a) == &False);

  mu_assert("Utils_ge: 123 >= 456 == False", A2(&Utils_ge, i123, i456) == &False);
  mu_assert("Utils_ge: 456 >= 123 == True", A2(&Utils_ge, i456, i123) == &True);
  mu_assert("Utils_ge: 123 >= 123 == True", A2(&Utils_ge, i123, i123a) == &True);

  return NULL;
}

char* utils_test() {
  if (verbose) {
    printf("\n\n\n");
    printf("####################################################\n");
    printf("\n");
    printf("Utils\n");
    printf("-----\n");
  }
  mu_run_test(test_records);
  mu_run_test(test_apply);
  mu_run_test(test_eq);
  mu_run_test(test_compare);

  return NULL;
}
