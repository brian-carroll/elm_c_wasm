#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "../kernel/types.h"
#include "./test.h"
#include "./types_test.h"

char* test_wasm_types() {
    if (verbose) {
        printf("sizeof(void*) = %d\n", (int)sizeof(void*));
        printf("sizeof(int) = %d\n", (int)sizeof(int));
        printf("\n");
        printf("sizeof(i8) = %d\n", (int)sizeof(i8));
        printf("sizeof(i16) = %d\n", (int)sizeof(i16));
        printf("sizeof(i32) = %d\n", (int)sizeof(i32));
        printf("sizeof(i64) = %d\n", (int)sizeof(i64));
        printf("sizeof(u8) = %d\n", (int)sizeof(u8));
        printf("sizeof(u16) = %d\n", (int)sizeof(u16));
        printf("sizeof(u32) = %d\n", (int)sizeof(u32));
        printf("sizeof(u64) = %d\n", (int)sizeof(u64));
        printf("sizeof(f32) = %d\n", (int)sizeof(f32));
        printf("sizeof(f64) = %d\n", (int)sizeof(f64));
        printf("\n");
    }
    mu_assert("i8 should be 1 byte", sizeof(i8)==1);
    mu_assert("i16 should be 2 bytes", sizeof(i16)==2);
    mu_assert("i32 should be 4 bytes", sizeof(i32)==4);
    mu_assert("i64 should be 8 bytes", sizeof(i64)==8);

    mu_assert("u8 should be 1 bytes", sizeof(u8)==1);
    mu_assert("u16 should be 2 bytes", sizeof(u16)==2);
    mu_assert("u32 should be 4 bytes", sizeof(u32)==4);
    mu_assert("u64 should be 8 bytes", sizeof(u64)==8);

    mu_assert("f32 should be 4 bytes", sizeof(f32)==4);
    mu_assert("f64 should be 8 bytes", sizeof(f64)==8);
    return NULL;
}


char* test_elm_constants() {
    if (verbose) {
        printf("Unit size=%ld addr=%s hex=%s\n", sizeof(Unit), hex_ptr(&Unit), hex(&Unit, sizeof(Unit)));
        printf("True size=%ld addr=%s hex=%s\n", sizeof(True), hex_ptr(&True), hex(&True, sizeof(True)));
        printf("False size=%ld addr=%s hex=%s\n", sizeof(False), hex_ptr(&False), hex(&False, sizeof(False)));
        printf("\n");
    }

    mu_assert("Unit should be 8 bytes wide", sizeof(Unit)==8);
    mu_assert("True should be 8 bytes wide", sizeof(True)==8);
    mu_assert("False should be 8 bytes wide", sizeof(False)==8);

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
    Header mask_tag = (Header){
        .tag = -1,
        .gc_mark = 0,
        .size = 0
    };
    Header mask_mark = (Header){
        .tag = 0,
        .gc_mark = -1,
        .size = 0
    };
    Header mask_size = (Header){
        .tag = 0,
        .gc_mark = 0,
        .size = -1
    };

    if (verbose) {
        printf("mask_tag  BE=%08x, LE=%s\n", *(u32*)&mask_tag,   hex(&mask_tag,   4));
        printf("mask_mark BE=%08x, LE=%s\n", *(u32*)&mask_mark, hex(&mask_mark, 4));
        printf("mask_size BE=%08x, LE=%s\n", *(u32*)&mask_size,  hex(&mask_size,  4));
        printf("\n");
    }

    mu_assert(
        "Header size should be 4 bytes",
        sizeof(Header) == 4
    );

    mu_assert(
        "Tag field should have 4 bits",
        count_flags(*(u32*)&mask_tag) == 4
    );

    mu_assert(
        "GC mark field should have 1 bit",
        count_flags(*(u32*)&mask_mark) == 1
    );

    mu_assert(
        "Size field should have 27 bits",
        count_flags(*(u32*)&mask_size) == 27
    );

    return NULL;
}

char* test_nil() {
    if (verbose) printf("Nil size=%ld addr=%s tag=%d\n", sizeof(Nil), hex_ptr(&Nil), (int)Nil.header.tag);
    mu_assert("Nil should be the same size as a header", sizeof(Nil) == sizeof(Header));
    mu_assert("Nil should have the right tag field", Nil.header.tag == Tag_Nil);
    mu_assert("Nil should have the right size field", Nil.header.size == 1);
    return NULL;
}

char* test_cons() {
    Cons *c = newCons(&Unit, &Nil); // [()]
    if (verbose) printf("Cons size=%ld addr=%s header.size=%d head=%s tail=%s, hex=%s\n",
        sizeof(Cons), hex_ptr(c), (int)c->header.size,
        hex_ptr(c->head), hex_ptr(c->tail),
        hex(c, sizeof(Cons))
    );

    #ifdef TARGET_64BIT
        mu_assert(
            "Cons struct should be the right size for a header, 4 bytes of padding, and 2 pointers",
            sizeof(Cons) == sizeof(Header) + 4 + 2*sizeof(void*)
        );
        mu_assert("newCons should insert correct size field", c->header.size == 6);
    #else
        mu_assert(
            "Cons struct should be the right size for a header and 2 pointers",
            sizeof(Cons) == sizeof(Header) + 2*sizeof(void*)
        );
        mu_assert("newCons should insert correct size field", c->header.size == 3);
    #endif

    mu_assert("[()] should have 'head' pointing to Unit", c->head == &Unit);
    mu_assert("[()] should have 'tail' pointing to Nil", c->tail == &Nil);

    free(c);
    return NULL;
}

char* test_tuples() {
    ElmInt *i1 = newElmInt(1);
    ElmInt *i2 = newElmInt(2);
    ElmInt *i3 = newElmInt(3);


    Tuple2 *t2 = newTuple2(&i1, &i2);
    if (verbose) printf("Tuple2 size=%ld addr=%s hex=%s\n",
        sizeof(Tuple2), hex_ptr(t2), hex(t2, sizeof(Tuple2))
    );
    #ifdef TARGET_64BIT
        mu_assert(
                "Tuple2 struct should be the right size for a header, 4 bytes of padding, and 2 pointers",
                sizeof(Tuple2) == sizeof(Header) + 4 + 2*sizeof(void*)
        );
        mu_assert("newTuple2 should insert correct size field", t2->header.size == 6);
    #else
        mu_assert(
                "Tuple2 struct should be the right size for a header and 2 pointers",
                sizeof(Tuple2) == sizeof(Header) + 2*sizeof(void*)
        );
        mu_assert("newTuple2 should insert correct size field", t2->header.size == 3);
    #endif
    mu_assert("Tuple2 should have correct tag field", t2->header.tag == Tag_Tuple2);
    mu_assert("(1,2) should have 'a' pointing to 1", t2->a == &i1);
    mu_assert("(1,2) should have 'b' pointing to 2", t2->b == &i2);


    Tuple3 *t3 = newTuple3(&i1, &i2, &i3);
    if (verbose) printf("Tuple3 size=%ld addr=%s hex=%s\n",
        sizeof(Tuple3), hex_ptr(t3), hex(t3, sizeof(Tuple3))
    );

    #ifdef TARGET_64BIT
        mu_assert(
                "Tuple3 struct should be the right size for a header, 4 bytes of padding, and 3 pointers",
                sizeof(Tuple3) == sizeof(Header) + 4 + 3*sizeof(void*)
        );
        mu_assert("newTuple3 should insert correct size field", t3->header.size == 8);
    #else
        mu_assert(
                "Tuple3 struct should be the right size for a header and 3 pointers",
                sizeof(Tuple3) == sizeof(Header) + 3*sizeof(void*)
        );
        mu_assert("newTuple3 should insert correct size field", t3->header.size == 4);
    #endif
    mu_assert("Tuple3 should have correct tag field", t3->header.tag == Tag_Tuple3);
    mu_assert("(1,2,3) should have 'a' pointing to 1", t3->a == &i1);
    mu_assert("(1,2,3) should have 'b' pointing to 2", t3->b == &i2);
    mu_assert("(1,2,3) should have 'c' pointing to 3", t3->c == &i3);


    free(i1);
    free(i2);
    free(i3);
    free(t2);
    free(t3);
    return NULL;
}

char* test_int() {
    ElmInt *i = newElmInt(123);

    if (verbose) printf("ElmInt size=%ld addr=%s tag=%d value=%d\n",
        sizeof(ElmInt), hex_ptr(i), (int)i->header.tag, i->value
    );
    mu_assert(
        "ElmInt type should be just wide enough for a header and an i32",
        sizeof(ElmInt) == sizeof(Header) + sizeof(i32)
    );
    mu_assert("newElmInt should insert correct tag field", i->header.tag == Tag_Int);
    mu_assert("newElmInt should insert correct size field", i->header.size == 2);
    mu_assert("newElmInt 123 should insert value of 123", i->value == 123);

    free(i);
    return NULL;
}

char* test_float() {
    ElmFloat *f = newElmFloat(123.456789);
    if (verbose) printf("Float size=%ld addr=%s tag=%d value=%f\n",
        sizeof(ElmFloat), hex_ptr(f), (int)f->header.tag, f->value
    );

    mu_assert("newElmFloat should insert correct tag field", f->header.tag == Tag_Float);
    mu_assert("newElmFloat should insert correct value", f->value == 123.456789);

    // f64 always gets aligned to 64-bit boundary, even for Wasm target
    mu_assert(
        "ElmFloat struct should be the right size for a header, 4 bytes of alignment padding, and an f64",
        sizeof(ElmFloat) == sizeof(Header) + 4 + sizeof(f64)
    );
    mu_assert("newElmFloat should insert correct size field", f->header.size == 4);

    free(f);
    return NULL;
}

char* test_char() {
    ElmChar *ch = newElmChar('A');
    if (verbose) printf("Char size=%ld addr=%s tag=%d value=%c\n",
        sizeof(ElmChar), hex_ptr(ch), (int)ch->header.tag, ch->value
    );

    mu_assert(
        "ElmChar type should be the right size for a header and an i32",
        sizeof(ElmChar) == sizeof(Header) + sizeof(i32)
    );
    mu_assert("ElmChar should have correct tag field", ch->header.tag == Tag_Char);
    mu_assert("ElmChar should have correct size field", ch->header.size == 2);
    mu_assert("ElmChar 'A' should have correct value", ch->value == 'A');

    free(ch);
    return NULL;
}


char* test_strings() {
    ElmString* str4 = newElmString(4, "1234");
    ElmString* str5 = newElmString(5, "12345");
    ElmString* str6 = newElmString(6, "123456");
    ElmString* str7 = newElmString(7, "1234567");
    ElmString* str8 = newElmString(8, "12345678");
    ElmString* strN = newElmString(45, "The quick brown fox jumped over the lazy dog.");

    if (verbose) {
        printf("sizeof(ElmString) = %ld\n", sizeof(ElmString));
        printf("&str4->bytes - str4 = %ld\n", (long)(&str4->bytes) - (long)(str4));
        printf("str4: tag=%d, size=%d, hex=%s\n", str4->header.tag, str4->header.size, hex(str4, sizeof(ElmString) + 8));
        printf("str5: tag=%d, size=%d, hex=%s\n", str5->header.tag, str5->header.size, hex(str5, sizeof(ElmString) + 8));
        printf("str6: tag=%d, size=%d, hex=%s\n", str6->header.tag, str6->header.size, hex(str6, sizeof(ElmString) + 8));
        printf("str7: tag=%d, size=%d, hex=%s\n", str7->header.tag, str7->header.size, hex(str7, sizeof(ElmString) + 8));
        printf("str8: tag=%d, size=%d, hex=%s\n", str8->header.tag, str8->header.size, hex(str8, sizeof(ElmString) + 12));
        printf("strN: tag=%d, size=%d, hex=%s\n", strN->header.tag, strN->header.size, hex(strN, sizeof(ElmString) + 48));
        printf("\n");
    }

    mu_assert("4-byte string should have correct body & padding", memcmp(str4->bytes, "1234\0\0\0\3", 8) == 0);
    mu_assert("5-byte string should have correct body & padding", memcmp(str5->bytes, "12345\0\0\2", 8) == 0);
    mu_assert("6-byte string should have correct body & padding", memcmp(str6->bytes, "123456\0\1", 8) == 0);
    mu_assert("7-byte string should have correct body & padding", memcmp(str7->bytes, "1234567\0", 8) == 0);
    mu_assert("8-byte string should have correct body & padding", memcmp(str8->bytes, "12345678\0\0\0\3", 12) == 0);
    mu_assert("45-byte string should have correct body & padding", memcmp(strN->bytes, "The quick brown fox jumped over the lazy dog.\0\0\2", 48) == 0);

    mu_assert("4-byte string should have correct size field", str4->header.size == 1 + 8/SIZE_UNIT);
    mu_assert("5-byte string should have correct size field", str5->header.size == 1 + 8/SIZE_UNIT);
    mu_assert("6-byte string should have correct size field", str6->header.size == 1 + 8/SIZE_UNIT);
    mu_assert("7-byte string should have correct size field", str7->header.size == 1 + 8/SIZE_UNIT);
    mu_assert("8-byte string should have correct size field", str8->header.size == 1 + 12/SIZE_UNIT);
    mu_assert("45-byte string should have correct size field", strN->header.size == 1 + 48/SIZE_UNIT);

    mu_assert("ElmString should have the correct type tag", str4->header.tag == Tag_String);

    free(str4);
    free(str5);
    free(str6);
    free(str7);
    free(str8);
    free(strN);
    return NULL;
}

char* test_custom() {
    Custom *c = malloc(sizeof(Custom) + 2*sizeof(void*));
    c->header = HEADER_CUSTOM(2);
    c->ctor = 1;
    c->values[0] = &Unit;
    c->values[1] = &Unit;
    if (verbose) {
        printf("custom with 2 values = %s\n", hex(c,  sizeof(Custom) + 2*sizeof(void*)));
    }

    mu_assert(
        "Custom struct with no parameters should be the size of a Header and an integer",
        sizeof(Custom)==sizeof(Header) + sizeof(u32)
    );
    #ifdef TARGET_64BIT
        mu_assert("HEADER_CUSTOM macro should insert correct size field", c->header.size == 6 ); // 1 + 2*2
    #else
        mu_assert("HEADER_CUSTOM macro should insert correct size field", c->header.size == 4 ); // 1 + 2
    #endif
    mu_assert("HEADER_CUSTOM macro should insert correct tag field", c->header.tag == Tag_Custom);


    free(c);
    return NULL;
}

char* test_record() {
    FieldSet *fs = malloc(sizeof(FieldSet) + 2*sizeof(u32));
    fs->size = 2;
    fs->fields[0] = 0xaa;
    fs->fields[1] = 0x55;

    Record *r = malloc(sizeof(Record) + 2*sizeof(void*));
    r->header = HEADER_RECORD(2);
    r->fieldset = fs;
    r->values[0] = &Unit;
    r->values[1] = &Nil;

    if (verbose) {
        printf("FieldSet with 2 values: addr=%s, hex=%s\n", hex_ptr(fs), hex(fs,  sizeof(FieldSet) + 2*sizeof(u32)));
        printf("Record with 2 values: tag=%d, hex=%s\n", r->header.tag, hex(r, sizeof(Record) + 2*sizeof(void*)));
    }

    mu_assert(
        "FieldSet struct with no fields should be the same size as an integer",
        sizeof(FieldSet) == sizeof(u32)
    );

    #ifdef TARGET_64BIT
        mu_assert(
            "Record struct with no fields should be the right size for a header, 4 bytes of padding, and 1 pointer",
            sizeof(Record) == sizeof(Header) + 4 + sizeof(void*)
        );
        mu_assert("HEADER_RECORD macro should insert correct size field", r->header.size == 8 ); // 2 + 3*2
    #else
        mu_assert(
            "Record struct with 2 fields should be the right size for a header and a pointer",
            sizeof(Record) == sizeof(Header) + sizeof(void*)
        );
        mu_assert("HEADER_RECORD macro should insert correct size field", r->header.size == 4 ); // 4*1
    #endif
    mu_assert("HEADER_RECORD macro should insert correct tag field", r->header.tag == Tag_Record);

    free(fs);
    free(r);
    return NULL;
}

void* dummy_evaluator(void** args) {
    return NULL;
}

char* test_closure() {
    Closure *c = malloc(sizeof(Closure) + 2*sizeof(void*));
    c->header = HEADER_CLOSURE(2);
    c->evaluator = &dummy_evaluator;
    c->max_values = 3;
    c->n_values = 2;
    c->values[0] = &Unit;
    c->values[1] = &Unit;
    if (verbose) {
        printf("Closure with 2 values = %s\n", hex(c,  sizeof(Closure) + 2*sizeof(void*)));
    }

    mu_assert(
        "Closure struct with no values should be the size of a Header, an integer, and a pointer",
        sizeof(Closure) == sizeof(Header) + sizeof(u32) + sizeof(void*)
    );
    #ifdef TARGET_64BIT
        mu_assert("HEADER_CLOSURE macro should insert correct size field", c->header.size == 8 ); // 1 + 1 + 2 + 2*2
    #else
        mu_assert("HEADER_CLOSURE macro should insert correct size field", c->header.size == 5 ); // 1 + 1 + 1 + 2
    #endif
    mu_assert("HEADER_CLOSURE macro should insert correct tag field", c->header.tag == Tag_Closure);

    free(c);
    return NULL;
}

char* types_test() {
    if (verbose) {
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
