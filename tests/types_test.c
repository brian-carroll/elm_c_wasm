#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
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
    return NULL;
}


char* test_elm_constants() {
    if (verbose) {
        printf("True size=%ld value=%d addr=%s\n", sizeof(True), True, hex_ptr(&True));
        printf("False size=%ld value=%d addr=%s\n", sizeof(False), False, hex_ptr(&False));
        printf("Unit size=%ld value=%d addr=%s\n", sizeof(Unit), Unit, hex_ptr(&Unit));
        printf("\n");
    }
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

    if (sizeof(Header) != 4) {
        return "Expected Header size to be 4 bytes";
    }

    if (count_flags(*(u32*)&mask_tag) != 4) {
        printf("test_header_layout: after if\n");
        return "Expected tag field to have 4 bits";
    }

    if (count_flags(*(u32*)&mask_color) != 2) {
        return "Expected color field to have 2 bits";
    }

    if (count_flags(*(u32*)&mask_size) != 26) {
        return "Expected size field to have 26 bits";
    }

    return NULL;
}


char* test_fixed_size_values() {
    if (verbose) printf("Nil size=%ld addr=%s ctor=%d\n", sizeof(Nil), hex_ptr(&Nil), (int)Nil.header.tag);

    Cons *c = newCons(&Unit, &Nil); // [()]
    if (verbose) printf("Cons size=%ld addr=%s ctor=%d head=%s tail=%s\n",
        sizeof(Cons), hex_ptr(c), (int)c->header.tag, hex_ptr(c->head), hex_ptr(c->tail)
    );
    free(c);

    Tuple2 *t2 = newTuple2(&Unit, &Unit); // ((),())
    if (verbose) printf("Tuple2 size=%ld addr=%s ctor=%d a=%s b=%s\n",
        sizeof(Tuple2), hex_ptr(t2), (int)t2->header.tag, hex_ptr(t2->a), hex_ptr(t2->b)
    );
    free(t2);

    Tuple3 *t3 = newTuple3(&Unit, &Unit, &Unit); // ((),(),())
    if (verbose) printf("Tuple3 size=%ld addr=%s ctor=%d a=%s b=%s c=%s\n",
        sizeof(Tuple3), hex_ptr(t3), (int)t3->header.tag, hex_ptr(t3->a), hex_ptr(t3->b), hex_ptr(t3->c)
    );
    free(t3);

    ElmInt *i = newElmInt(123);
    if (verbose) printf("Int size=%ld addr=%s ctor=%d value=%d\n",
        sizeof(ElmInt), hex_ptr(i), (int)i->header.tag, i->value
    );
    free(i);

    ElmFloat *f = newElmFloat(123.456789);
    if (verbose) printf("Float size=%ld addr=%s ctor=%d value=%f\n",
        sizeof(ElmFloat), hex_ptr(f), (int)f->header.tag, f->value
    );
    free(f);

    ElmChar *ch = newElmChar('A');
    if (verbose) printf("Char size=%ld addr=%s ctor=%d value=%c\n",
        sizeof(ElmChar), hex_ptr(ch), (int)ch->header.tag, ch->value
    );
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
        printf("str4: tag=%d, size=%d, hex=%s\n", str4->header.tag, str4->header.size, hex(str4, sizeof(ElmString) + 8));
        printf("str5: tag=%d, size=%d, hex=%s\n", str5->header.tag, str5->header.size, hex(str5, sizeof(ElmString) + 8));
        printf("str6: tag=%d, size=%d, hex=%s\n", str6->header.tag, str6->header.size, hex(str6, sizeof(ElmString) + 8));
        printf("str7: tag=%d, size=%d, hex=%s\n", str7->header.tag, str7->header.size, hex(str7, sizeof(ElmString) + 8));
        printf("str8: tag=%d, size=%d, hex=%s\n", str8->header.tag, str8->header.size, hex(str8, sizeof(ElmString) + 12));
        printf("strN: tag=%d, size=%d, hex=%s\n", strN->header.tag, strN->header.size, hex(strN, sizeof(ElmString) + 48));
        printf("\n");
    }
    return NULL;
}


char* test_all() {
    mu_run_test(test_wasm_types);
    mu_run_test(test_elm_constants);
    mu_run_test(test_header_layout);
    mu_run_test(test_fixed_size_values);
    mu_run_test(test_strings);
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
