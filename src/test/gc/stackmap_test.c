#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../../kernel/types.h"
#include "../../kernel/gc.h"
#include "../../kernel/gc-internals.h"

extern GcState gc_state;

const int MAX_LINES = 50;

struct heap_item_spec
{
    int idx;
    Tag tag;
    int depth;
    int backlink;
    bool mark;
    bool replay;
    void *addr;
};

void parse_heap_item_spec(char *line, struct heap_item_spec *spec)
{
    int idx;
    char tag[10];
    int depth;
    int backlink = 0;
    char mark[6];
    char replay[6];
    int cols = sscanf(
        line,
        "%d\t%s\t%d\t%s\t%d\t%s",
        &idx,
        tag,
        &depth,
        mark,
        &backlink,
        replay);

    printf("row idx %d: read %d columns\n", idx, cols);

    if (!strcmp(tag, "empty"))
    {
        spec->tag = Tag_GcStackEmpty;
    }
    else if (!strcmp(tag, "push"))
    {
        spec->tag = Tag_GcStackPush;
    }
    else if (!strcmp(tag, "pop"))
    {
        spec->tag = Tag_GcStackPop;
    }
    else if (!strcmp(tag, "allocate"))
    {
        spec->tag = Tag_Float;
    }
    else if (!strcmp(tag, "tailcall"))
    {
        spec->tag = Tag_GcStackTailCall;
    }
    else if (!strcmp(tag, "exception"))
    {
        spec->tag = Tag_GcException;
    }
    else
    {
        fprintf(stderr, "Unknown type tag '%s'\n", tag);
        exit(EXIT_FAILURE);
    }

    spec->idx = idx;
    spec->depth = depth;
    spec->backlink = backlink;
    spec->mark = strcmp(mark, "TRUE") == 0;
    spec->replay = strcmp(replay, "TRUE") == 0;
    spec->addr = NULL;
}

void parse_heap_spec_file(char *filename, struct heap_item_spec heap_spec[])
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Could not read %s\n", filename);
        exit(EXIT_FAILURE);
    }
    int i = -1;
    while ((read = getline(&line, &len, fp)) != -1)
    {
        if (i > -1) // skip header line
            parse_heap_item_spec(line, &heap_spec[i]);
        i++;
    }
    heap_spec[i].idx = -1; // EOF marker

    fclose(fp);
    if (line)
    {
        free(line);
    }
}

void format_addr(void *addr, char s[15])
{
    sprintf(s, "%04llx_%04llx_%04llx",
            ((u64)addr & 0xffff00000000) >> 32,
            ((u64)addr & 0x0000ffff0000) >> 16,
            ((u64)addr & 0x00000000ffff));
}

int find_idx_from_pointer(void *p, struct heap_item_spec heap_spec[])
{
    for (int i = 0; (heap_spec[i].idx >= 0) && (i < MAX_LINES); i++)
    {
        if (p == heap_spec[i].addr)
        {
            return i;
        }
    }
    return -1;
}

bool addr_is_marked(void *p)
{
    GcState *state = &gc_state;
    size_t *pword = (size_t *)p;
    size_t slot = pword - state->heap.start;
    if (slot >> (GC_WORD_BITS - 1))
        return true; // off heap => not garbage, stop tracing
    size_t word = slot / GC_WORD_BITS;
    size_t bit = slot % GC_WORD_BITS;
    size_t mask = (size_t)1 << bit;

    return (state->heap.bitmap[word] & mask) != 0;
}

void print_heap_spec_item(struct heap_item_spec heap_spec[], int idx)
{
    struct heap_item_spec *spec = &heap_spec[idx];
    char *tag_names[16] = {
        "Int            ",
        "Float          ",
        "Char           ",
        "String         ",
        "Nil            ",
        "Cons           ",
        "Tuple2         ",
        "Tuple3         ",
        "Custom         ",
        "Record         ",
        "Closure        ",
        "GcException    ",
        "GcStackEmpty   ",
        "GcStackPush    ",
        "GcStackPop     ",
        "GcStackTailCall",
        //234567890123456
    };
    char addr[16];
    char link[16];
    char mark[3];
    sprintf(addr, "           ");
    sprintf(link, "           "); // not only NULL case, also non-stackmap
    sprintf(mark, "  ");

    if (spec->addr)
    {
        format_addr(spec->addr, addr);

        bool mark_bit = addr_is_marked(spec->addr);
        sprintf(mark, "%c%c",
                mark_bit ? 'X' : ' ',
                mark_bit == spec->mark ? ' ' : '!');

        switch (spec->tag)
        {
        case Tag_GcStackPush:
        case Tag_GcStackPop:
        case Tag_GcStackTailCall:
        {
            GcStackMap *stackmap = (GcStackMap *)spec->addr;
            format_addr(stackmap->older, link);
            if (spec->backlink)
            {
                int actual_backlink = find_idx_from_pointer(stackmap->older, heap_spec);
                if (actual_backlink != spec->backlink)
                    fprintf(stderr,
                            "Heap incorrectly populated. %d should link back to %d, not %d\n",
                            spec->idx, spec->backlink, actual_backlink);
            }
        }
        }
    }

    char backlink[4];
    if (spec->backlink)
        sprintf(backlink, "%3d", spec->backlink);
    else
        sprintf(backlink, "   ");

    printf("%14s  %14s  %2s  |  %3d  %15s  %3d     %c    %3s      %c\n",
           addr,
           link,
           mark,
           //------
           spec->idx,
           tag_names[spec->tag],
           spec->depth,
           spec->mark ? 'X' : ' ',
           backlink,
           spec->replay ? 'X' : ' ');
}

void print_heap_spec(struct heap_item_spec heap_spec[])
{
    printf("\n");
    printf("         ACTUAL                     |                     SPEC\n");
    printf("                                    |\n");
    printf("    address          link      mark |  idx   tag             depth  mark  link  replay\n");
    printf("--------------  -------------- ---- |  ---  ---------------  -----  ----  ----  ------\n");
    for (int i = 0; (heap_spec[i].idx >= 0) && (i < MAX_LINES); i++)
    {
        print_heap_spec_item(heap_spec, i);
    }
}

struct heap_item_spec *populate_heap_from_spec(struct heap_item_spec *spec)
{
    static void *last_alloc = NULL;
    void *push = NULL;
    while (spec->idx >= 0)
    {
        switch (spec->tag)
        {
        case Tag_GcStackEmpty:
        {
            spec->addr = gc_state.stack_map;
            spec++;
            break;
        }
        case Tag_GcStackPush:
        {
            bool isTailcall;
            push = GC_stack_push();
            spec->addr = push;
            spec++;
            do
            {
                spec = populate_heap_from_spec(spec);
                if (spec->tag == Tag_GcException)
                    return spec;
                isTailcall = spec->tag == Tag_GcStackTailCall;
                if (isTailcall)
                {
                    size_t n_args = 2;
                    size_t size = sizeof(Closure) + n_args * sizeof(void *);
                    Closure *c = GC_malloc(size);
                    spec->addr = GC_stack_tailcall(c, push);
                }
                else
                {
                    spec->addr = GC_stack_pop(last_alloc, push);
                }
                spec++;
            } while (isTailcall);
            break;
        }
        case Tag_GcStackPop:
            return spec;

        case Tag_GcStackTailCall:
            return spec;

        case Tag_Float:
        {
            spec->addr = ctorElmFloat(123.456);
            last_alloc = spec->addr;
            spec++;
            break;
        }
        case Tag_GcException:
            return spec;
        default:
            fprintf(stderr, "Can't populate heap with unhandled tag 0x%x", spec->tag);
            exit(EXIT_FAILURE);
        } // switch
    }     // loop
    return spec;
}

int main(int argc, char **argv)
{
    struct heap_item_spec heap_spec[MAX_LINES];
    char *filename;

    GC_init();
    Types_init();

    if (argc != 2)
    {
        fprintf(stderr, "Expect exactly one argument, the file to read. Got %d\n", argc - 1);
        exit(EXIT_FAILURE);
    }
    printf("Reading %s ...\n", argv[1]);
    filename = argv[1];

    parse_heap_spec_file(filename, heap_spec);
    printf("spec parsed\n");
    print_heap_spec(heap_spec);
    printf("populating the heap...\n");
    populate_heap_from_spec(heap_spec);
    printf("heap populated\n");
    print_heap_spec(heap_spec);
    printf("marking stack map\n");
    mark_stack_map(&gc_state, gc_state.heap.start);
    print_heap_spec(heap_spec);

    exit(EXIT_SUCCESS);
}
