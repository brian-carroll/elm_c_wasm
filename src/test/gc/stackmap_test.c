#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../../kernel/types.h"
#include "../../kernel/gc.h"
#include "../../kernel/gc-internals.h"

extern GcState gc_state;
bool verbose = true;

#define MAX_SPEC_LINES 50
#define MAX_ERROR_LEN (8 * 1024)

typedef struct
{
    int idx;
    Tag tag;
    int depth;
    int backlink;
    bool mark;
    bool replay;
    void *addr;
} HeapSpecLine;

typedef struct
{
    char *filename;
    HeapSpecLine lines[MAX_SPEC_LINES];
    int length;
    char *err_buf;
    int err_idx;
} HeapSpec;

void append_error(char *err, HeapSpec *spec)
{
    int len = strlen(err);
    int new_len = spec->err_idx + len;
    if (new_len < MAX_ERROR_LEN)
    {
        char *destination = &spec->err_buf[spec->err_idx];
        strcpy(destination, err);
        spec->err_idx += len;
    }
}

void parse_heap_spec_line(char *line, HeapSpecLine *line_spec)
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

    if (verbose)
        printf("row idx %d: read %d columns\n", idx, cols);

    if (!strcmp(tag, "empty"))
    {
        line_spec->tag = Tag_GcStackEmpty;
    }
    else if (!strcmp(tag, "push"))
    {
        line_spec->tag = Tag_GcStackPush;
    }
    else if (!strcmp(tag, "pop"))
    {
        line_spec->tag = Tag_GcStackPop;
    }
    else if (!strcmp(tag, "allocate"))
    {
        line_spec->tag = Tag_Float;
    }
    else if (!strcmp(tag, "tailcall"))
    {
        line_spec->tag = Tag_GcStackTailCall;
    }
    else if (!strcmp(tag, "exception"))
    {
        line_spec->tag = Tag_GcException;
    }
    else
    {
        fprintf(stderr, "Unknown type tag '%s'\n", tag);
        exit(EXIT_FAILURE);
    }

    line_spec->idx = idx;
    line_spec->depth = depth;
    line_spec->backlink = backlink;
    line_spec->mark = strcmp(mark, "TRUE") == 0;
    line_spec->replay = strcmp(replay, "TRUE") == 0;
    line_spec->addr = NULL;
}

void parse_heap_spec_file(char *filename, HeapSpec *spec)
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    spec->filename = filename;

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
            parse_heap_spec_line(line, &spec->lines[i]);
        i++;
    }
    spec->length = i;

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

int find_idx_from_pointer(void *p, HeapSpec *spec)
{
    for (int i = 0; i < spec->length; i++)
    {
        if (p == spec->lines[i].addr)
            return i;
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

void validate_heap_item(HeapSpec *spec, int idx)
{
    HeapSpecLine *line = &spec->lines[idx];
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
    sprintf(link, "           "); // default when addr is NULL or line not a stackmap
    sprintf(mark, "  ");

    if (line->addr)
    {
        format_addr(line->addr, addr);

        bool mark_bit = addr_is_marked(line->addr);
        bool mark_has_error = mark_bit != line->mark;
        sprintf(mark, "%c%c",
                mark_bit ? 'X' : ' ',
                mark_has_error ? '!' : ' ');
        if (mark_has_error)
            fprintf(stderr, "Heap index %d should%s be marked\n",
                    idx, line->mark ? "" : " not");

        switch (line->tag)
        {
        case Tag_GcStackPush:
        case Tag_GcStackPop:
        case Tag_GcStackTailCall:
        {
            GcStackMap *stackmap = (GcStackMap *)line->addr;
            format_addr(stackmap->older, link);
            if (line->backlink)
            {
                int actual_backlink = find_idx_from_pointer(stackmap->older, spec);
                if (actual_backlink != line->backlink)
                    fprintf(stderr,
                            "Heap incorrectly populated. %d should link back to %d, not %d\n",
                            line->idx, line->backlink, actual_backlink);
            }
        }
        }
    }

    char backlink[4];
    if (line->backlink)
        sprintf(backlink, "%3d", line->backlink);
    else
        sprintf(backlink, "   ");

    if (verbose)
        printf("%14s  %14s  %2s  |  %3d  %15s  %3d     %c    %3s      %c\n",
               addr,
               link,
               mark,
               //------
               line->idx,
               tag_names[line->tag],
               line->depth,
               line->mark ? 'X' : ' ',
               backlink,
               line->replay ? 'X' : ' ');
}

void validate_heap(HeapSpec *spec)
{
    if (verbose)
    {
        printf("\n");
        printf("         ACTUAL                     |                     SPEC\n");
        printf("                                    |\n");
        printf("    address          link      mark |  idx   tag             depth  mark  link  replay\n");
        printf("--------------  -------------- ---- |  ---  ---------------  -----  ----  ----  ------\n");
    }
    for (int i = 0; i < spec->length; i++)
    {
        validate_heap_item(spec, i);
    }
}

HeapSpecLine *populate_heap_from_spec(HeapSpecLine *line, HeapSpecLine *last_line)
{
    static void *last_alloc = NULL;
    void *push = NULL;
    while (line <= last_line)
    {
        switch (line->tag)
        {
        case Tag_GcStackEmpty:
        {
            line->addr = gc_state.stack_map;
            line++;
            break;
        }
        case Tag_GcStackPush:
        {
            bool isTailcall;
            push = GC_stack_push();
            line->addr = push;
            line++;
            do
            {
                line = populate_heap_from_spec(line, last_line);
                if (line->tag == Tag_GcException)
                    return line;
                isTailcall = line->tag == Tag_GcStackTailCall;
                if (isTailcall)
                {
                    size_t n_args = 2;
                    size_t size = sizeof(Closure) + n_args * sizeof(void *);
                    Closure *c = GC_malloc(size);
                    line->addr = GC_stack_tailcall(c, push);
                }
                else
                {
                    line->addr = GC_stack_pop(last_alloc, push);
                }
                line++;
            } while (isTailcall);
            break;
        }
        case Tag_GcStackPop:
            return line;

        case Tag_GcStackTailCall:
            return line;

        case Tag_Float:
        {
            line->addr = ctorElmFloat(123.456);
            last_alloc = line->addr;
            line++;
            break;
        }
        case Tag_GcException:
            return line;
        default:
            fprintf(stderr, "Can't populate heap with unhandled tag 0x%x", line->tag);
            exit(EXIT_FAILURE);
        } // switch
    }     // loop
    return line;
}

char *test_stackmap_spec(char *filename, char *err_buf)
{
    HeapSpec spec;

    memset(&spec, 0, sizeof(spec));
    spec.err_buf = err_buf;

    if (verbose)
        printf("Reading heap spec '%s'\n\n", filename);

    parse_heap_spec_file(filename, &spec);

    if (verbose)
        printf("spec parsed, populating the heap...\n");

    populate_heap_from_spec(spec.lines, &spec.lines[spec.length - 1]);

    if (verbose)
        printf("heap populated, marking stack map...\n");

    mark_stack_map(&gc_state, gc_state.heap.start);

    if (verbose)
        printf("stack map marked, validating heap...\n");

    sscanf(spec.err_buf, "%s:\n", filename);
    int start_idx = strlen(spec.err_buf);
    spec.err_idx = start_idx;

    validate_heap(&spec);

    if (spec.err_idx > start_idx)
        return spec.err_buf;
    else
        return NULL;
}

char *test_stackmap()
{
    char *spec_filenames[] = {
        "stackmap_data/full_completion.tsv",
        "stackmap_data/throw_3_deep_completed.tsv",
        "stackmap_data/throw_below_tailcall.tsv",
        "stackmap_data/throw_3_calls_deep.tsv",
        "stackmap_data/throw_3_deep_tailcall.tsv",
        "stackmap_data/throw_one_call_deep.tsv",
        NULL,
    };
    char err_buf[MAX_ERROR_LEN];
    char *err;
    for (int i = 0; spec_filenames[i] != NULL; i++)
    {
        memset(err_buf, 0, MAX_ERROR_LEN);
        err = test_stackmap_spec(spec_filenames[i], err_buf);
        if (err != NULL)
            return err;
        if (verbose)
            printf("\n\n---------------------\n\n");
    }
    return NULL;
}

int main(int argc, char **argv)
{
    char *filename;
    char *err;

    GC_init();
    Types_init();

    err = test_stackmap();
    if (err)
    {
        fputs(err, stderr);
        exit(EXIT_FAILURE);
    }
    else
    {
        exit(EXIT_SUCCESS);
    }
}
