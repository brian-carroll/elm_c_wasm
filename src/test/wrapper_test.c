#include "../kernel/kernel.h"
#include "./test.h"

extern FieldGroup fg_init_subscriptions_update_view;

#define TEST_ARRAY_LEN 17

static void populate_test_array(void* test_values[]) {
  ElmString* str_kernel = create_string("kernel");
  ElmString* str_array = create_string("array");
  ElmString* str_union = create_string("union");
  Custom* kernel_array = newCustom(JSON_VALUE_ARRAY, 2, (void*[]){str_kernel, str_array});
  Tuple2* tuple2_UV = newTuple2(create_string("U"), create_string("V"));
  Tuple3* tuple3_XYZ =
      newTuple3(create_string("X"), create_string("Y"), create_string("Z"));
  Cons* list123 = List_create(3, (void*[]){newElmInt(1), newElmInt(2), newElmInt(3)});
  Record* knownRecord = newRecord(&fg_init_subscriptions_update_view,
      4,
      (void*[]){
          create_string("init value"),
          create_string("subscriptions value"),
          create_string("update value"),
          create_string("view value"),
      });

#if 0
  Header fgHeader = HEADER_FIELDGROUP(2);
  FieldGroup* unknownFieldGroup = GC_allocate(true, fgHeader.size);
  int next_field_id = ARRAY_LEN(Wrapper_appFieldGroups);
  unknownFieldGroup->header = fgHeader;
  unknownFieldGroup->size = 2;
  unknownFieldGroup->fields[0] = next_field_id;
  unknownFieldGroup->fields[1] = next_field_id + 1;
  Record* unknownRecord = newRecord(unknownFieldGroup,
      2,
      (void*[]){
          create_string("bar value"),  // fields sorted alphabetically
          create_string("foo value"),
      });
#endif

  Custom* kernelUnion = newCustom(KERNEL_CTOR_OFFSET + 5,
      5,
      (void*[]){
          /* a */ NULL,
          /* b */ str_kernel,
          /* c */ NULL,
          /* d */ NULL,
          /* e */ str_union,
      });

  Custom* just321 = A1(&g_elm_core_Maybe_Just, newElmInt(321));

  int i = 0;
  /*  0 */ test_values[i++] = NULL;
  /*  1 */ test_values[i++] = &Json_encodeNull;
  /*  2 */ test_values[i++] = newElmInt(123);
  /*  3 */ test_values[i++] = newElmFloat(3.14);
  /*  4 */ test_values[i++] = create_string("hello world");
  /*  5 */ test_values[i++] = &True;
  /*  6 */ test_values[i++] = &False;
  /*  7 */ test_values[i++] = newElmChar('x');
  /*  8 */ test_values[i++] = kernel_array;
  /*  9 */ test_values[i++] = &Nil;
  /* 10 */ test_values[i++] = &Unit;
  /* 11 */ test_values[i++] = tuple2_UV;
  /* 12 */ test_values[i++] = tuple3_XYZ;
  /* 13 */ test_values[i++] = list123;
  /* 14 */ test_values[i++] = knownRecord;
  /* 15 */ test_values[i++] = kernelUnion;
  /* 16 */ test_values[i++] = just321;
}

static void* eval_createTuple3(void* args[]) {
  return newTuple3(args[0], args[1], args[2]);
}

// --------------------------------------------------


static void test_write_values() {
  void* test_values[TEST_ARRAY_LEN];
  populate_test_array(test_values);
  char message[100];

  for (int i = 0; i < TEST_ARRAY_LEN; i++) {
    void* actual = testWriteJsValueToWasm(i);
    void* expected = test_values[i];
    stbsp_sprintf(message, "should correctly write JS test value #%d", i);
    expect_equal(message, actual, expected);
    if (verbose) {
      print_value(expected);
      safe_printf("\n\n");
    }
  }
}


static void test_call_wasm_from_js() {
  Closure* c = newClosure(1, 3, eval_createTuple3, (void*[]){&True});
  Tuple3* actual = testCallWasmFuncWithJsArgs(c);
  Tuple3* expected =
      A2(c, newElmInt(999), A1(&g_elm_core_Maybe_Just, create_string("hello")));
  expect_equal(
      "should correctly apply 2 JS arguments to a Wasm closure", actual, expected);
}


static void test_call_js_from_wasm() {
  Closure* jsKernelIncrementBy1 = testWriteJsCallbackToWasm();
  Closure* thunk = A1(jsKernelIncrementBy1, newElmInt(100));
  ElmInt* actual = testElmValueRoundTrip(thunk);  // evaluate the thunk in JS

  ElmInt* expected = newElmInt(101);
  expect_equal("should be able to call a JS function from Wasm", actual, expected);
}


static void test_read() {
  void* test_values[TEST_ARRAY_LEN];
  populate_test_array(test_values);
  char message[100];

  for (int i = 0; i < TEST_ARRAY_LEN; i++) {
    void* actual = testElmValueRoundTrip(test_values[i]);
    void* expected = test_values[i];
    stbsp_sprintf(message, "should correctly read JS test value #%d", i);
    expect_equal(message, actual, expected);
    if (verbose) {
      print_value(expected);
      safe_printf("\n\n");
    }
  }
}

void wrapper_test() {
  if (verbose) {
    safe_printf("\n\n\n");
    safe_printf("####################################################\n");
    safe_printf("\n");
    safe_printf("JS Wrapper\n");
    safe_printf("==========\n");
  }
  describe("test_write_values", test_write_values);
  describe("test_call_wasm_from_js", test_call_wasm_from_js);
  describe("test_call_js_from_wasm", test_call_js_from_wasm);
  describe("test_read", test_read);
}
