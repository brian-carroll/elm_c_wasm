#include <stdio.h>
#include "../src/types.h"
#include "./test.c"


int main(int argc, char ** argv) {

    // Memory layout

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

    printf("False size=%ld %s %d\n", sizeof(False), hex_ptr(&False), False);
    printf("True size=%ld %s %d\n", sizeof(True), hex_ptr(&True), True);
    printf("Unit size=%ld %s %d\n", sizeof(Unit), hex_ptr(&Unit), Unit);
    printf("\n");

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
    printf("mask_tag   BE=%08x, LE=%s\n", *(u32*)&mask_tag,   hex(&mask_tag,   4));
    printf("mask_color BE=%08x, LE=%s\n", *(u32*)&mask_color, hex(&mask_color, 4));
    printf("mask_size  BE=%08x, LE=%s\n", *(u32*)&mask_size,  hex(&mask_size,  4));
    printf("\n");

    printf("Nil size=%ld addr=%s ctor=%d\n", sizeof(Nil), hex_ptr(&Nil), (int)Nil.header.tag);

    Cons *c = newCons(&Unit, &Nil); // [()]
    printf("Cons size=%ld addr=%s ctor=%d head=%d tail=%d\n",
        sizeof(Cons), hex_ptr(c), (int)c->header.tag, (int)c->head, (int)c->tail
    );
    free(c);

    Tuple2 *t2 = newTuple2(&Unit, &Unit); // ((),())
    printf("Tuple2 size=%ld addr=%s ctor=%d a=%d b=%d\n",
        sizeof(Tuple2), hex_ptr(t2), (int)t2->header.tag, (int)t2->a, (int)t2->b
    );
    free(t2);

    Tuple3 *t3 = newTuple3(&Unit, &Unit, &Unit); // ((),(),())
    printf("Tuple3 size=%ld addr=%s ctor=%d a=%d b=%d c=%d\n",
        sizeof(Tuple3), hex_ptr(t3), (int)t3->header.tag, (int)t3->a, (int)t3->b, (int)t3->c
    );
    free(t3);

    ElmInt *i = newElmInt(123);
    printf("Int size=%ld addr=%s ctor=%d value=%d\n",
        sizeof(ElmInt), hex_ptr(i), (int)i->header.tag, i->value
    );
    free(i);

    ElmFloat *f = newElmFloat(123.456);
    printf("Float size=%ld addr=%s ctor=%d value=%f\n",
        sizeof(ElmFloat), hex_ptr(f), (int)f->header.tag, f->value
    );
    free(f);

    ElmChar *ch = newElmChar('A');
    printf("Char size=%ld addr=%s ctor=%d value=%c\n",
        sizeof(ElmChar), hex_ptr(ch), (int)ch->header.tag, ch->value
    );
    free(ch);
    printf("\n");

    
    ElmString* str4 = newElmString(4, "1234");
    ElmString* str5 = newElmString(5, "12345");
    ElmString* str6 = newElmString(6, "123456");
    ElmString* str7 = newElmString(7, "1234567");
    ElmString* str8 = newElmString(8, "12345678");
    ElmString* strN = newElmString(45, "The quick brown fox jumped over the lazy dog.");

    printf("str4: tag=%d, size=%d, hex=%s\n", str4->header.tag, str4->header.size, hex(str4, sizeof(ElmString) + 8));
    printf("str5: tag=%d, size=%d, hex=%s\n", str5->header.tag, str5->header.size, hex(str5, sizeof(ElmString) + 8));
    printf("str6: tag=%d, size=%d, hex=%s\n", str6->header.tag, str6->header.size, hex(str6, sizeof(ElmString) + 8));
    printf("str7: tag=%d, size=%d, hex=%s\n", str7->header.tag, str7->header.size, hex(str7, sizeof(ElmString) + 8));
    printf("str8: tag=%d, size=%d, hex=%s\n", str8->header.tag, str8->header.size, hex(str8, sizeof(ElmString) + 12));
    printf("strN: tag=%d, size=%d, hex=%s\n", strN->header.tag, strN->header.size, hex(strN, sizeof(ElmString) + 48));
}
