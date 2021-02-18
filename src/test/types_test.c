#include "../kernel/core/types.h"
#include <string.h>
#include "test.h"

#ifndef _MSC_VER
#include <unistd.h>
#endif

char* test_wasm_types() {
  if (verbose) {
    printf("\n");
    printf("## test_wasm_types\n");
#ifdef TARGET_64BIT
    printf("Target platform is 64-bit\n");
#else
    printf("Target platform is 32-bit\n");
#endif
    printf("sizeof(void*) = %d\n", (int)sizeof(void*));
    printf("sizeof(size_t) = %d\n", (int)sizeof(size_t));
    printf("sizeof(int) = %d\n", (int)sizeof(int));
  }
  mu_assert("i8 should be 1 byte", sizeof(i8) == 1);
  mu_assert("i16 should be 2 bytes", sizeof(i16) == 2);
  mu_assert("i32 should be 4 bytes", sizeof(i32) == 4);
  mu_assert("i64 should be 8 bytes", sizeof(i64) == 8);

  mu_assert("u8 should be 1 bytes", sizeof(u8) == 1);
  mu_assert("u16 should be 2 bytes", sizeof(u16) == 2);
  mu_assert("u32 should be 4 bytes", sizeof(u32) == 4);
  mu_assert("u64 should be 8 bytes", sizeof(u64) == 8);

  mu_assert("f32 should be 4 bytes", sizeof(f32) == 4);
  mu_assert("f64 should be 8 bytes", sizeof(f64) == 8);
  return NULL;
}

char* test_elm_constants() {
  if (verbose) {
    printf("\n");
    printf("## test_elm_constants\n");
    printf("Unit size=%zd addr=%s hex=%s\n",
        sizeof(Unit),
        hex_ptr(&Unit),
        hex(&Unit, sizeof(Unit)));
    printf("True size=%zd addr=%s hex=%s\n",
        sizeof(True),
        hex_ptr(&True),
        hex(&True, sizeof(True)));
    printf("False size=%zd addr=%s hex=%s\n",
        sizeof(False),
        hex_ptr(&False),
        hex(&False, sizeof(False)));
  }

  mu_assert("Unit should be sizeof(Custom)", sizeof(Unit) == sizeof(Custom));
  mu_assert("True should be sizeof(Custom)", sizeof(True) == sizeof(Custom));
  mu_assert("False should be sizeof(Custom)", sizeof(False) == sizeof(Custom));

  return NULL;
}

int count_flags(u32 flags) {
  u32 count = 0;
  while (flags) {
    count += flags & 1;
    flags = flags >> 1;
  }
  return count;
}

char* test_header_layout() {
  Header mask_tag = (Header){.tag = -1, .size = 0};
  Header mask_size = (Header){.tag = 0, .size = -1};

  if (verbose) {
    printf("\n");
    printf("## test_header_layout\n");
    printf("mask_tag  BE=%08x, LE=%s\n", *(u32*)&mask_tag, hex(&mask_tag, 4));
    printf("mask_size BE=%08x, LE=%s\n", *(u32*)&mask_size, hex(&mask_size, 4));
  }

  mu_assert("Header size should be 4 bytes", sizeof(Header) == 4);
  mu_assert("Tag field should have 4 bits", count_flags(*(u32*)&mask_tag) == 4);
  mu_assert("Size field should have 28 bits", count_flags(*(u32*)&mask_size) == 28);

  return NULL;
}

char* test_nil() {
  if (verbose) {
    printf("\n## test_nil\n");
    printf("Nil sizeof=%zd addr=%s tag=%d, hex=%s\n",
        sizeof(Nil),
        hex_ptr(&Nil),
        (int)Nil.header.tag,
        hex(&Nil, sizeof(Nil)));
  }
  mu_assert("Nil should be the same size as Cons", sizeof(Nil) == sizeof(Cons));
  mu_assert("Nil should have the right tag field", Nil.header.tag == Tag_List);
  mu_assert("Nil should have the right size field", Nil.header.size == 3);
  return NULL;
}

char* test_cons() {
  if (verbose) printf("\n## test_cons\n");
  Cons* c = ctorCons(&Unit, &Nil);  // [()]
  if (verbose)
    printf("Cons size=%zd addr=%s header.size=%d head=%s tail=%s, hex=%s\n",
        sizeof(Cons),
        hex_ptr(c),
        (int)c->header.size,
        hex_ptr(c->head),
        hex_ptr(c->tail),
        hex(c, sizeof(Cons)));

#ifdef TARGET_64BIT
  mu_assert(
      "Cons struct should be the right size for a header, 4 bytes of padding, and 2 "
      "pointers",
      sizeof(Cons) == sizeof(Header) + 4 + 2 * sizeof(void*));
#else
  mu_assert("Cons struct should be the right size for a header and 2 pointers",
      sizeof(Cons) == sizeof(Header) + 2 * sizeof(void*));
#endif
  mu_assert("ctorCons should insert correct size field", c->header.size == 3);

  mu_assert("[()] should have 'head' pointing to Unit", c->head == &Unit);
  mu_assert("[()] should have 'tail' pointing to Nil", c->tail == &Nil);

  return NULL;
}

char* test_tuples() {
  if (verbose) printf("\n## test_tuples\n");
  ElmInt* i1 = ctorElmInt(1);
  ElmInt* i2 = ctorElmInt(2);
  ElmInt* i3 = ctorElmInt(3);

  Tuple2* t2 = ctorTuple2(&i1, &i2);
  if (verbose)
    printf("Tuple2 sizeof=%zd header.size=%d hex=%s\n",
        sizeof(Tuple2),
        t2->header.size,
        hex(t2, sizeof(Tuple2)));
#ifdef TARGET_64BIT
  mu_assert(
      "Tuple2 struct should be the right size for a header, 4 bytes of padding, and 2 "
      "pointers",
      sizeof(Tuple2) == sizeof(Header) + 4 + 2 * sizeof(void*));
#else
  mu_assert("Tuple2 struct should be the right size for a header and 2 pointers",
      sizeof(Tuple2) == sizeof(Header) + 2 * sizeof(void*));
#endif
  mu_assert("ctorTuple2 should insert correct size field", t2->header.size == 3);
  mu_assert("Tuple2 should have correct tag field", t2->header.tag == Tag_Tuple2);
  mu_assert("(1,2) should have 'a' pointing to 1", t2->a == &i1);
  mu_assert("(1,2) should have 'b' pointing to 2", t2->b == &i2);

  Tuple3* t3 = ctorTuple3(&i1, &i2, &i3);
  if (verbose)
    printf("Tuple3 size=%zd header.size=%d hex=%s\n",
        sizeof(Tuple3),
        t3->header.size,
        hex(t3, sizeof(Tuple3)));

#ifdef TARGET_64BIT
  mu_assert(
      "Tuple3 struct should be the right size for a header, 4 bytes of padding, and 3 "
      "pointers",
      sizeof(Tuple3) == sizeof(Header) + 4 + 3 * sizeof(void*));
#else
  mu_assert("Tuple3 struct should be the right size for a header and 3 pointers",
      sizeof(Tuple3) == sizeof(Header) + 3 * sizeof(void*));
#endif
  mu_assert("ctorTuple3 should insert correct size field", t3->header.size == 4);
  mu_assert("Tuple3 should have correct tag field", t3->header.tag == Tag_Tuple3);
  mu_assert("(1,2,3) should have 'a' pointing to 1", t3->a == &i1);
  mu_assert("(1,2,3) should have 'b' pointing to 2", t3->b == &i2);
  mu_assert("(1,2,3) should have 'c' pointing to 3", t3->c == &i3);

  return NULL;
}

char* test_int() {
  if (verbose) printf("\n## test_int\n");
  ElmInt* i = ctorElmInt(123);

  if (verbose)
    printf("ElmInt size=%zd addr=%s tag=%d value=%d\n",
        sizeof(ElmInt),
        hex_ptr(i),
        (int)i->header.tag,
        i->value);
  mu_assert("ElmInt type should be just wide enough for a header and an i32",
      sizeof(ElmInt) == sizeof(Header) + sizeof(i32));
  mu_assert("ctorElmInt should insert correct tag field", i->header.tag == Tag_Int);
  mu_assert("ctorElmInt 123 should insert value of 123", i->value == 123);
#ifdef TARGET_64BIT
  mu_assert("ctorElmInt should insert correct size field", i->header.size == 1);
#else
  mu_assert("ctorElmInt should insert correct size field", i->header.size == 2);
#endif

  return NULL;
}

char* test_float() {
  if (verbose) printf("\n## test_float\n");
  ElmFloat* f = ctorElmFloat(123.456789);
  if (verbose)
    printf("Float size=%zd addr=%s tag=%d value=%f\n",
        sizeof(ElmFloat),
        hex_ptr(f),
        (int)f->header.tag,
        f->value);

  mu_assert("ctorElmFloat should insert correct tag field", f->header.tag == Tag_Float);
  mu_assert("ctorElmFloat should insert correct value", f->value == 123.456789);

  // f64 always gets aligned to 64-bit boundary, even for Wasm target
  mu_assert(
      "ElmFloat struct should be the right size for a header, 4 bytes of alignment "
      "padding, and an f64",
      sizeof(ElmFloat) == sizeof(Header) + 4 + sizeof(f64));
#ifdef TARGET_64BIT
  mu_assert("ctorElmFloat should insert correct size field", f->header.size == 2);
#else
  mu_assert("ctorElmFloat should insert correct size field", f->header.size == 4);
#endif

  return NULL;
}

char* test_char() {
  if (verbose) printf("\n## test_char\n");
  ElmChar* ch = ctorElmChar('A');
  if (verbose)
    printf("Char size=%zd addr=%s tag=%d value=%c\n",
        sizeof(ElmChar),
        hex_ptr(ch),
        (int)ch->header.tag,
        ch->value);

  mu_assert("ElmChar type should be the right size for a header and an i32",
      sizeof(ElmChar) == sizeof(Header) + sizeof(i32));
  mu_assert("ElmChar should have correct tag field", ch->header.tag == Tag_Char);
  mu_assert("ElmChar 'A' should have correct value", ch->value == 'A');
#ifdef TARGET_64BIT
  mu_assert("ctorElmChar should insert correct size field", ch->header.size == 1);
#else
  mu_assert("ctorElmChar should insert correct size field", ch->header.size == 2);
#endif

  return NULL;
}

char* test_strings() {
  ElmString* str0 = ctorElmString(0, "");
  ElmString* str1 = ctorElmString(1, "1");
  ElmString* str2 = ctorElmString(2, "12");
  ElmString* str3 = ctorElmString(3, "123");
  ElmString* str4 = ctorElmString(4, "1234");
  ElmString* str5 = ctorElmString(5, "12345");
  ElmString* str7 = ctorElmString(7, "1234567");
  ElmString* str8 = ctorElmString(8, "12345678");
  ElmString* str9 = ctorElmString(9, "123456789");
  char s[] = "The quick brown fox jumped over the lazy dog, yeah.";
  size_t n = strlen(s);
  ElmString* strN = ctorElmString(n, s);

  if (verbose) {
    printf("\n");
    printf("## test_strings\n");
    printf("\n");
    printf("sizeof(ElmString) = %zd\n", sizeof(ElmString));
    printf("\n");
  }

#ifdef TARGET_64BIT
  mu_expect_equal("0-byte string should have correct size field", str0->header.size, 1);
  mu_expect_equal("1-byte string should have correct size field", str1->header.size, 1);
  mu_expect_equal("2-byte string should have correct size field", str2->header.size, 1);
  mu_expect_equal("3-byte string should have correct size field", str3->header.size, 1);
  mu_expect_equal("4-byte string should have correct size field", str4->header.size, 1);
  mu_expect_equal("5-byte string should have correct size field", str5->header.size, 2);
  mu_expect_equal("7-byte string should have correct size field", str7->header.size, 2);
  mu_expect_equal("8-byte string should have correct size field", str8->header.size, 2);
  mu_expect_equal("9-byte string should have correct size field", str9->header.size, 2);
  mu_expect_equal("N-byte string should have correct size field", strN->header.size, 7);
#else
  mu_expect_equal("0-byte string should have correct size field", str0->header.size, 1);
  mu_expect_equal("1-byte string should have correct size field", str1->header.size, 2);
  mu_expect_equal("2-byte string should have correct size field", str2->header.size, 2);
  mu_expect_equal("3-byte string should have correct size field", str3->header.size, 2);
  mu_expect_equal("4-byte string should have correct size field", str4->header.size, 2);
  mu_expect_equal("5-byte string should have correct size field", str5->header.size, 3);
  mu_expect_equal("7-byte string should have correct size field", str7->header.size, 3);
  mu_expect_equal("8-byte string should have correct size field", str8->header.size, 3);
  mu_expect_equal("9-byte string should have correct size field", str9->header.size, 4);
  mu_expect_equal("51-byte string should have correct size field",
      strN->header.size,
      1 + ((n + 3) / 4));
#endif

  mu_assert("ctorElmString should insert the correct type tag",
      str9->header.tag == Tag_String);

  return NULL;
}

char* test_custom() {
  u8 memory[sizeof(Custom) + 2 * sizeof(void*)];
  Custom* c = (Custom*)memory;
  c->header = (Header)HEADER_CUSTOM(2);
  c->ctor = 1;
  c->values[0] = &Unit;
  c->values[1] = &Unit;
  if (verbose) {
    printf("\n");
    printf("## test_custom\n");
    printf("custom with 2 values = %s\n", hex(c, sizeof(Custom) + 2 * sizeof(void*)));
  }

  mu_assert(
      "Custom struct with no parameters should be the size of a Header and an integer",
      sizeof(Custom) == sizeof(Header) + sizeof(u32));
#ifdef TARGET_64BIT
  mu_assert("HEADER_CUSTOM macro should insert correct size field",
      c->header.size == 3);  // 0.5 + 0.5 + 2*1
#else
  mu_assert("HEADER_CUSTOM macro should insert correct size field",
      c->header.size == 4);  // 1 + 1 + 2
#endif
  mu_assert(
      "HEADER_CUSTOM macro should insert correct tag field", c->header.tag == Tag_Custom);

  return NULL;
}

char* test_record() {
  u8 mem_fs[sizeof(FieldGroup) + 2 * sizeof(u32)];
  FieldGroup* fg = (FieldGroup*)mem_fs;
  fg->size = 2;
  fg->fields[0] = 0xaa;
  fg->fields[1] = 0x55;

  u8 mem_r[sizeof(Record) + 2 * sizeof(void*)];
  Record* r = (Record*)mem_r;
  r->header = (Header)HEADER_RECORD(2);
  r->fieldgroup = fg;
  r->values[0] = &Unit;
  r->values[1] = &Nil;

  if (verbose) {
    printf("\n");
    printf("## test_record\n");
    printf("FieldGroup with 2 values: addr=%s, hex=%s\n",
        hex_ptr(fg),
        hex(fg, sizeof(FieldGroup) + 2 * sizeof(u32)));
    printf("Record with 2 values: tag=%d, size=%d, hex=%s\n",
        r->header.tag,
        r->header.size,
        hex(r, sizeof(Record) + 2 * sizeof(void*)));
  }

#ifdef TARGET_64BIT
  mu_assert(
      "Record struct with no fields should be the right size for a header, 4 bytes of "
      "padding, and 1 pointer",
      sizeof(Record) == sizeof(Header) + 4 + sizeof(void*));
  mu_assert("HEADER_RECORD macro should insert correct size field",
      r->header.size == 4);  // 0.5 + 0.5 + 1 + 2
#else
  mu_assert(
      "Record struct with 2 fields should be the right size for a header and a pointer",
      sizeof(Record) == sizeof(Header) + sizeof(void*));
  mu_assert("HEADER_RECORD macro should insert correct size field",
      r->header.size == 4);  // 4*1
#endif
  mu_assert(
      "HEADER_RECORD macro should insert correct tag field", r->header.tag == Tag_Record);

  return NULL;
}

void* dummy_evaluator(void** args) {
  return NULL;
}

char* test_closure() {
  u8 mem_c[sizeof(Closure) + 2 * sizeof(void*)];
  Closure* c = (Closure*)mem_c;
  c->header = (Header)HEADER_CLOSURE(2);
  c->evaluator = &dummy_evaluator;
  c->max_values = 3;
  c->n_values = 2;
  c->values[0] = &Unit;
  c->values[1] = &Unit;
  if (verbose) {
    printf("\n");
    printf("## test_closure\n");
    printf("Closure with 2 values = %s\n", hex(c, sizeof(Closure) + 2 * sizeof(void*)));
  }

  mu_assert(
      "Closure struct with no values should be the size of a Header, an integer, and a "
      "pointer",
      sizeof(Closure) == sizeof(Header) + sizeof(u32) + sizeof(void*));
#ifdef TARGET_64BIT
  mu_assert("HEADER_CLOSURE macro should insert correct size field",
      c->header.size == 4);  // 0.5 + 0.25 + 0.25 + 1 + 1 + 1
#else
  mu_assert("HEADER_CLOSURE macro should insert correct size field",
      c->header.size == 5);  // 1 + 1 + 1 + 2
#endif
  mu_assert("HEADER_CLOSURE macro should insert correct tag field",
      c->header.tag == Tag_Closure);

  return NULL;
}

char* types_test() {
  if (verbose) {
    printf("\n");
    printf("####################################################\n");
    printf("\n");
    printf("Elm type structures\n");
    printf("-------------------\n\n");
  }

  mu_run_test(test_wasm_types);
  mu_run_test(test_elm_constants);
  mu_run_test(test_header_layout);

  mu_run_test(test_nil);
  mu_run_test(test_cons);
  mu_run_test(test_tuples);
  mu_run_test(test_int);
  mu_run_test(test_float);
  mu_run_test(test_char);
  if (verbose) printf("\n");

  mu_run_test(test_strings);
  mu_run_test(test_custom);
  mu_run_test(test_record);
  mu_run_test(test_closure);
  return NULL;
}
