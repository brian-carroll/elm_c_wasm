#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../../kernel/types.h"
#include "../../kernel/gc.h"
#include "../../kernel/gc-internals.h"

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

void parse_heap_item_spec(char *line, struct heap_item_spec *item)
{
    int idx;
    char tag[10];
    int depth;
    int backlink = 0;
    char mark[6];
    char replay[6];
    sscanf(
        line,
        "%d\t%s\t%d\t%d\t%s\t%s",
        &idx,
        tag,
        &depth,
        &backlink,
        mark,
        replay);

    if (!strcmp(tag, "empty"))
    {
        item->tag = Tag_GcStackEmpty;
    }
    else if (!strcmp(tag, "push"))
    {
        item->tag = Tag_GcStackPush;
    }
    else if (!strcmp(tag, "pop"))
    {
        item->tag = Tag_GcStackPop;
    }
    else if (!strcmp(tag, "allocate"))
    {
        item->tag = Tag_Float;
    }
    else if (!strcmp(tag, "tailcall"))
    {
        item->tag = Tag_GcStackTailCall;
    }
    else
    {
        fprintf(stderr, "Unknown type tag '%s'\n", tag);
        exit(EXIT_FAILURE);
    }

    item->idx = idx;
    item->depth = depth;
    item->backlink = backlink;
    item->mark = strcmp(mark, "TRUE") == 0;
    item->replay = strcmp(replay, "TRUE") == 0;
    item->addr = NULL;
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
        if (i == -1)
        {
            i++;
            continue;
        }
        printf("%s", line);
        struct heap_item_spec *item = &heap_spec[i];
        parse_heap_item_spec(line, item);
        i++;
    }
    heap_spec[i].idx = -1; // EOF marker

    fclose(fp);
    if (line)
    {
        free(line);
    }
}

void print_heap_spec_item(struct heap_item_spec heap_spec[], int idx)
{
    struct heap_item_spec *item = &heap_spec[idx];
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
    };
    printf("%012llx\t%3d\t%s\t%3d\t%5d   \t%s\t%s\n",
           (long long int)item->addr,
           item->idx,
           tag_names[item->tag],
           item->depth,
           item->backlink,
           item->mark ? "TRUE" : "FALSE",
           item->replay ? "TRUE" : "FALSE");
}

void print_heap_spec(struct heap_item_spec heap_spec[])
{
    printf("\n");
    printf("  address   	idx	 tag           	depth	backlink	mark	replay\n");
    printf("----------- 	---	---------------	-----	--------	----	------\n");
    for (int i = 0; (heap_spec[i].idx >= 0) && (i < 50); i++)
    {
        print_heap_spec_item(heap_spec, i);
    }
}

extern GcState gc_state;

struct heap_item_spec *populate_heap_from_spec(struct heap_item_spec *item)
{
    while (item->idx >= 0)
    {
        void *last_alloc = NULL;
        void *push = NULL;

        printf("populating item %d, tag %x\n", item->idx, item->tag);

        switch (item->tag)
        {
        case Tag_GcStackEmpty:
            item->addr = gc_state.stack_map;
            item++;
            break;
        case Tag_GcStackPush:
        {
            item->addr = GC_stack_push();
            push = item->addr;
            item = populate_heap_from_spec(item + 1);
            break;
        }
        case Tag_GcStackPop:
        {
            item->addr = GC_stack_pop(last_alloc, push);
            if (item->depth <= 0)
            {
                fprintf(stderr, "popping from depth %d\n", item->depth);
            }
            item++;
            return item;
        }
        case Tag_GcStackTailCall:
        {
            size_t n_args = 2;
            size_t size = sizeof(Closure) + n_args * sizeof(void *);
            Closure *c = GC_malloc(size);
            item->addr = GC_stack_tailcall(c, push);
            item++;
        }
        break;
        case Tag_Float:
        {
            item->addr = ctorElmFloat(123.456);
            last_alloc = item->addr;
            item++;
        }
        break;
        default:
            fprintf(stderr, "Can't populate heap with unhandled tag 0x%x", item->tag);
            exit(EXIT_FAILURE);
        } // switch
    }     // loop
    return item;
}

int main(int argc, char **argv)
{
    struct heap_item_spec heap_spec[50];
    char *filename;

    GC_init();
    Types_init();
    // Utils_init();

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

    exit(EXIT_SUCCESS);
}
