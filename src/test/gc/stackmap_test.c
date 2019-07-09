#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../../kernel/types.h"
#include "../../kernel/gc.h"
#include "../../kernel/gc-internals.h"
#include "../gc_test.h"
#include "../test.h"

// Generated C arrays, converted from TSV files
#include "stackmap_data/full_completion.inc"
#include "stackmap_data/throw_3_calls_deep.inc"
#include "stackmap_data/throw_3_deep_completed.inc"
#include "stackmap_data/throw_3_deep_tailcall.inc"
#include "stackmap_data/throw_below_tailcall.inc"
#include "stackmap_data/throw_one_call_deep.inc"

extern GcState gc_state;
extern int verbose;

#define MAX_SPEC_LINES 50
#define MAX_LINE_LEN 200
#define MAX_ERROR_LEN (8 * 1024)
#define MAX_SINGLE_ERR_LEN 256

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
    u8 *text;
    u32 text_len;
    HeapSpecLine lines[MAX_SPEC_LINES];
    int n_lines;
    char err_buf[MAX_ERROR_LEN];
    int err_idx;
} HeapSpec;

void append_error(HeapSpec *spec, char *err)
{
    int len = strlen(err) + 1;
    int new_len = spec->err_idx + len;
    if (new_len < MAX_ERROR_LEN)
    {
        char *destination = &spec->err_buf[spec->err_idx];
        sprintf(destination, "\t%s", err);
        spec->err_idx += len;
    }
}

void parse_heap_spec_line(u8 *line, HeapSpec *spec, int line_idx)
{
    int idx;
    char tag[10];
    int depth;
    int backlink = 0;
    char mark[6];
    char replay[6];
    char err[MAX_SINGLE_ERR_LEN];
    HeapSpecLine *line_spec = &spec->lines[line_idx];

    // int cols =
    sscanf(
        (char *)line,
        "%d\t%s\t%d\t%s\t%d\t%s",
        &idx,
        tag,
        &depth,
        mark,
        &backlink,
        replay);

    if (idx != line_idx)
    {
        sprintf(err, "Wrong index. Found %d, should be %d\n", idx, line_idx);
        append_error(spec, err);
    }

    // if (verbose)
    //     printf("row idx %d: read %d columns\n", idx, cols);

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
        sprintf(err, "Unknown type tag '%s'\n", tag);
        append_error(spec, err);
    }

    line_spec->idx = idx;
    line_spec->depth = depth;
    line_spec->backlink = backlink;
    line_spec->mark = strcmp(mark, "TRUE") == 0;
    line_spec->replay = strcmp(replay, "TRUE") == 0;
    line_spec->addr = NULL;
}

void parse_heap_spec(HeapSpec *spec)
{
    u8 *text = spec->text;
    u32 len = spec->text_len;
    u8 *line = text;
    int line_idx = -1;

    for (u32 c = 0; c < len; c++)
    {
        if (text[c] == '\n')
        {
            text[c] = 0;
            if (line_idx > -1)
                parse_heap_spec_line(line, spec, line_idx);
            line_idx++;
            line = &text[c + 1];
        }
    }
    spec->n_lines = line_idx;
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
    for (int i = 0; i < spec->n_lines; i++)
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
    char err[MAX_SINGLE_ERR_LEN];

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
        {
            sprintf(err, "Heap index %d should%s be marked\n",
                    idx, line->mark ? "" : " not");
            append_error(spec, err);
        }

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
                {
                    sprintf(err,
                            "Heap incorrectly populated. %d should link back to %d, not %d\n",
                            line->idx, line->backlink, actual_backlink);
                    append_error(spec, err);
                }
            }
            break;
        }

        default:
            break;
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
    for (int i = 0; i < spec->n_lines; i++)
    {
        validate_heap_item(spec, i);
    }
}

HeapSpecLine *populate_heap_from_spec(HeapSpecLine *line, HeapSpec *spec)
{
    static void *last_alloc = NULL;
    void *push = NULL;
    char err[MAX_SINGLE_ERR_LEN];
    HeapSpecLine *last_line = &spec->lines[spec->n_lines - 1];

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
                line = populate_heap_from_spec(line, spec);
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
        {
            sprintf(err, "Can't populate heap with unhandled tag 0x%x", line->tag);
            append_error(spec, err);
        }
        } // switch
    }     // loop
    return line;
}

void test_stackmap_spec(HeapSpec *spec)
{
    if (verbose)
        printf("Reading heap spec '%s'\n\n", spec->filename);

    parse_heap_spec(spec);

    if (verbose)
        printf("spec parsed, populating the heap...\n");

    gc_test_reset();
    populate_heap_from_spec(spec->lines, spec);

    if (verbose)
        printf("heap populated, marking stack map...\n");

    mark_stack_map(&gc_state, gc_state.heap.start);

    if (verbose)
        printf("stack map marked, validating heap...\n");

    validate_heap(spec);
}

#define NUM_SPEC_FILES 6

char *stackmap_mark_test()
{
    int failed_specs = 0;

    HeapSpec specs[NUM_SPEC_FILES] = {
        {
            .filename = "full_completion.tsv",
            .text = full_completion_tsv,
            .text_len = full_completion_tsv_len,
        },
        {
            .filename = "throw_3_deep_completed.tsv",
            .text = throw_3_deep_completed_tsv,
            .text_len = throw_3_deep_completed_tsv_len,
        },
        {
            .filename = "throw_below_tailcall.tsv",
            .text = throw_below_tailcall_tsv,
            .text_len = throw_below_tailcall_tsv_len,
        },
        {
            .filename = "throw_3_calls_deep.tsv",
            .text = throw_3_calls_deep_tsv,
            .text_len = throw_3_calls_deep_tsv_len,
        },
        {
            .filename = "throw_3_deep_tailcall.tsv",
            .text = throw_3_deep_tailcall_tsv,
            .text_len = throw_3_deep_tailcall_tsv_len,
        },
        {
            .filename = "throw_one_call_deep.tsv",
            .text = throw_one_call_deep_tsv,
            .text_len = throw_one_call_deep_tsv_len,
        },
    };

    if (verbose)
    {
        printf("\n\n\n");
        printf("########################################################################################\n");
        printf("stackmap_mark_test\n");
        printf("------------------\n");
        printf("\n");
        printf(" - Fill up the heap with a data pattern specified by one of %d .tsv files\n", NUM_SPEC_FILES);
        printf(" - Heap now looks as if Elm functions have been running. Throw a GC 'heap full' exception.\n");
        printf(" - Check if the correct values are marked:\n");
        printf("     - Values allocated by still-running function calls\n");
        printf("     - Values returned from completed function calls\n");
        printf("\n");
    }

    for (int i = 0; i < NUM_SPEC_FILES; i++)
    {
        HeapSpec *spec = &specs[i];
        test_stackmap_spec(spec);
        bool pass = !spec->err_buf[0];
        if (!pass)
            failed_specs++;
        if (verbose)
            printf("\n%s\n%s\n---------------------\n\n",
                   pass ? "PASS" : "FAIL",
                   spec->err_buf);
    }
    if (verbose)
    {
        printf("\nSUMMARY\n");
        printf("%d test specs\n", NUM_SPEC_FILES);
        printf("%d passed\n", NUM_SPEC_FILES - failed_specs);
        printf("%d failed\n", failed_specs);
        printf("\n\n");
    }
    if (failed_specs)
    {
        fprintf(stderr, "\nSTACKMAP TEST FAILURES\n----------------------\n");
        for (int i = 0; i < NUM_SPEC_FILES; i++)
            if (specs[i].err_buf[0])
            {
                fprintf(stderr, "%s:\n%s\n",
                        specs[i].filename,
                        specs[i].err_buf);
                if (verbose)
                    fprintf(stdout, "%s:\n%s\n",
                            specs[i].filename,
                            specs[i].err_buf);
            }
    }
    return failed_specs ? "GC stackmap should match all 6 specs" : NULL;
}
