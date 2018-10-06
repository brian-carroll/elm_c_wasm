#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include "../src/types.h"
#include "./test.c" // including .c file, naughty!

bool verbose = false;
int tests_run = 0;

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
        printf("True size=%ld value=%d addr=%s\n", sizeof(True), True, hex_ptr(&True));
        printf("False size=%ld value=%d addr=%s\n", sizeof(False), False, hex_ptr(&False));
        printf("Unit size=%ld value=%d addr=%s\n", sizeof(Unit), Unit, hex_ptr(&Unit));
        printf("\n");
    }

    mu_assert("True should be 1 byte wide", sizeof(True)==1);
    mu_assert("False should be 1 byte wide", sizeof(False)==1);
    mu_assert("Unit should be 1 byte wide", sizeof(Unit)==1);

    mu_assert("True should have value 1", True==1);
    mu_assert("False should have value 0", False==0);
    mu_assert("Unit should have value 0", Unit==0);

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
        .gc_color = 0,
        .size = 0
    };
    Header mask_color = (Header){
        .tag = 0,
        .gc_color = -1,
        .size = 0
    };
    Header mask_size = (Header){
        .tag = 0,
        .gc_color = 0,
        .size = -1
    };

    if (verbose) {
        printf("mask_tag   BE=%08x, LE=%s\n", *(u32*)&mask_tag,   hex(&mask_tag,   4));
        printf("mask_color BE=%08x, LE=%s\n", *(u32*)&mask_color, hex(&mask_color, 4));
        printf("mask_size  BE=%08x, LE=%s\n", *(u32*)&mask_size,  hex(&mask_size,  4));
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
        "GC color field should have 2 bits",
        count_flags(*(u32*)&mask_color) == 2
    );

    mu_assert(
        "Size field should have 26 bits",
        count_flags(*(u32*)&mask_size) == 26
    );

    return NULL;
}


char* test_fixed_size_values() {
    if (verbose) printf("Nil size=%ld addr=%s ctor=%d\n", sizeof(Nil), hex_ptr(&Nil), (int)Nil.header.tag);

    Cons *c = newCons(&Unit, &Nil); // [()]
    if (verbose) printf("Cons size=%ld addr=%s header.size=%d head=%s tail=%s, hex=%s\n",
        sizeof(Cons), hex_ptr(c), (int)c->header.size,
        hex_ptr(c->head), hex_ptr(c->tail),
        hex(c, sizeof(Cons))
    );
    // On a 64-bit target, gcc adds 32 bits of padding between .header and .head
    // (which aligns the pointers to a 64-bit boundary)
    // Tests should pass for 32 (Wasm) or 64 (handy test target + maybe future back-end Elm)
    mu_assert(
        "Cons type should be wide enough for a header and two pointers",
        sizeof(Cons) >= sizeof(Header) + 2*sizeof(void*)
    );
    mu_assert(
        "Cons type should no wider than 3 pointers",
        sizeof(Cons) <= 3*sizeof(void*)
    );
    mu_assert("Cons should have correct header tag", c->header.tag == Tag_Cons);
    #ifdef TARGET_64BIT
        mu_assert("Cons should have correct size field", c->header.size == 5);
    #else
        mu_assert("Cons should have correct size field", c->header.size == 3);
    #endif
    mu_assert("[()] should have 'head' pointing to Unit", c->head == &Unit);
    mu_assert("[()] should have 'tail' pointing to Nil", c->tail == &Nil);
    free(c);

    ElmInt *i1 = newElmInt(1);
    ElmInt *i2 = newElmInt(2);
    ElmInt *i3 = newElmInt(3);

    Tuple2 *t2 = newTuple2(&i1, &i2);
    if (verbose) printf("Tuple2 size=%ld addr=%s hex=%s\n",
        sizeof(Tuple2), hex_ptr(t2), hex(t2, sizeof(Tuple2))
    );
    mu_assert(
        "Tuple2 type should be wide enough for a header and two pointers",
        sizeof(Tuple2) >= sizeof(Header) + 2*sizeof(void*)
    );
    mu_assert(
        "Tuple2 type should no wider than 3 pointers",
        sizeof(Tuple2) <= 3*sizeof(void*)
    );
    #ifdef TARGET_64BIT
        mu_assert("Tuple2 should have correct size field", t2->header.size == 5);
    #else
        mu_assert("Tuple2 should have correct size field", t2->header.size == 3);
    #endif
    mu_assert("Tuple2 should have correct header tag", t2->header.tag == Tag_Tuple2);
    mu_assert("(1,2) should have 'a' pointing to 1", t2->a == &i1);
    mu_assert("(1,2) should have 'b' pointing to 2", t2->b == &i2);
    free(t2);

    Tuple3 *t3 = newTuple3(&i1, &i2, &i3);
    if (verbose) printf("Tuple3 size=%ld addr=%s hex=%s\n",
        sizeof(Tuple3), hex_ptr(t3), hex(t3, sizeof(Tuple3))
    );
    mu_assert(
        "Tuple3 type should be wide enough for a header and 3 pointers",
        sizeof(Tuple3) >= sizeof(Header) + 3*sizeof(void*)
    );
    mu_assert(
        "Tuple3 type should no wider than 4 pointers",
        sizeof(Tuple3) <= 4*sizeof(void*)
    );
    #ifdef TARGET_64BIT
        mu_assert("Tuple3 should have correct size field", t3->header.size == 7);
    #else
        mu_assert("Tuple3 should have correct size field", t3->header.size == 4);
    #endif
    mu_assert("Tuple3 should have correct header tag", t3->header.tag == Tag_Tuple3);
    mu_assert("(1,2,3) should have 'a' pointing to 1", t3->a == &i1);
    mu_assert("(1,2,3) should have 'b' pointing to 2", t3->b == &i2);
    mu_assert("(1,2,3) should have 'c' pointing to 3", t3->c == &i3);
    free(t3);

    ElmInt *i = newElmInt(123);
    if (verbose) printf("Int size=%ld addr=%s ctor=%d value=%d\n",
        sizeof(ElmInt), hex_ptr(i), (int)i->header.tag, i->value
    );
    mu_assert(
        "ElmInt type should be just wide enough for a header and an i32",
        sizeof(ElmInt) == sizeof(Header) + sizeof(i32)
    );
    mu_assert("ElmInt should have correct header tag", i->header.tag == Tag_Int);
    mu_assert("ElmInt should have correct header size", i->header.size == 1);
    mu_assert("123 should have value of 123", i->value == 123);
    free(i);

    ElmFloat *f = newElmFloat(123.456789);
    if (verbose) printf("Float size=%ld addr=%s ctor=%d value=%f\n",
        sizeof(ElmFloat), hex_ptr(f), (int)f->header.tag, f->value
    );
    mu_assert(
        "ElmFloat type should be just wide enough for a header and an f64",
        sizeof(ElmFloat) >= sizeof(Header) + sizeof(f64)
    );
    mu_assert(
        "ElmFloat may have up to 4 bytes of padding",
        sizeof(ElmFloat) <= sizeof(Header) + sizeof(f64) + 4
    );
    mu_assert("ElmFloat should have correct header tag", f->header.tag == Tag_Float);
    #ifdef TARGET_64BIT
        mu_assert("ElmFloat should have correct header size", f->header.size == 3);
    #else
        mu_assert("ElmFloat should have correct header size", f->header.size == 2);
    #endif
    mu_assert("123.456789 should have value of 123", f->value == 123.456789);
    free(f);

    ElmChar *ch = newElmChar('A');
    if (verbose) printf("Char size=%ld addr=%s ctor=%d value=%c\n",
        sizeof(ElmChar), hex_ptr(ch), (int)ch->header.tag, ch->value
    );
    mu_assert(
        "ElmChar type should be just wide enough for a header and an i32",
        sizeof(ElmChar) == sizeof(Header) + sizeof(i32)
    );
    mu_assert("ElmChar should have correct header tag", ch->header.tag == Tag_Char);
    mu_assert("ElmChar should have correct header size", ch->header.size == 1);
    mu_assert("ElmChar 'A' should have correct value", ch->value == 'A');
    free(ch);

    if (verbose) printf("\n");
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

    mu_assert("4-byte string should have correct body & padding", memcmp(str4->bytes, "1234\0\0\0\4", 8) == 0);
    mu_assert("5-byte string should have correct body & padding", memcmp(str5->bytes, "12345\0\0\3", 8) == 0);
    mu_assert("6-byte string should have correct body & padding", memcmp(str6->bytes, "123456\0\2", 8) == 0);
    mu_assert("7-byte string should have correct body & padding", memcmp(str7->bytes, "1234567\1", 8) == 0);
    mu_assert("8-byte string should have correct body & padding", memcmp(str8->bytes, "12345678\0\0\0\4", 12) == 0);
    mu_assert("45-byte string should have correct body & padding", memcmp(strN->bytes, "The quick brown fox jumped over the lazy dog.\0\0\3", 48) == 0);

    mu_assert("4-byte string should have correct header size", str4->header.size == 8/SIZE_UNIT);
    mu_assert("5-byte string should have correct header size", str5->header.size == 8/SIZE_UNIT);
    mu_assert("6-byte string should have correct header size", str6->header.size == 8/SIZE_UNIT);
    mu_assert("7-byte string should have correct header size", str7->header.size == 8/SIZE_UNIT);
    mu_assert("8-byte string should have correct header size", str8->header.size == 12/SIZE_UNIT);
    mu_assert("45-byte string should have correct header size", strN->header.size == 48/SIZE_UNIT);

    mu_assert("ElmString should have the correct type tag", str4->header.tag == Tag_String);

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
    r->values[1] = &Unit;
    if (verbose) {
        printf("FieldSet with 2 values: addr=%s, hex=%s\n", hex_ptr(fs), hex(fs,  sizeof(FieldSet) + 2*sizeof(u32)));
        printf("Record with 2 values = %s\n", hex(r,  sizeof(Record) + 2*sizeof(void*)));
    }
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
    return NULL;

}

char* test_all() {
    mu_run_test(test_wasm_types);
    mu_run_test(test_elm_constants);
    mu_run_test(test_header_layout);
    mu_run_test(test_fixed_size_values);
    mu_run_test(test_strings);
    mu_run_test(test_custom);
    mu_run_test(test_record);
    mu_run_test(test_closure);
    return NULL;
}


int main(int argc, char ** argv) {
    int opt;

    while ((opt = getopt(argc, argv, "v")) != -1) {
        switch (opt) {
            case 'v':
                verbose = true;
                break;
            default:
                fprintf(stderr, "Usage: %s [-v]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    char* result = test_all();
    bool passed = result == NULL;

    if (!passed) {
        printf("%s\n", result);
    } else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);

    return !passed;
}
