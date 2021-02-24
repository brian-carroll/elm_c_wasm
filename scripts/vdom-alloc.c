#include "../src/kernel/kernel.h"

typedef u16 Index;

struct index_range {
  Index begin;
  Index end;
};
typedef struct index_range IndexRange;


/*
comparing node tags
- check if on heap or not!
- if not then pointer comparison alone is enough, no need to deref
- normally they will be off-heap
- Even if user code has "div" string literal, compiler will share it.
- User could do "d" ++ "iv" in which case it will be on heap and we do full comparison
*/

struct vdom_node_soa {
  ElmString16** tags; // we can have a few specific tag strings for other node types
  IndexRange* facts;
  IndexRange* children;
  IndexRange* extras; // namespaces, keys, text, taggers, lazy thunks
  struct vdom_node_soa* next;
};

struct vdom_state {
  struct vdom_node_soa* nodes;
  void** node_to_node;
  void** node_to_fact;
};

/*

OK all this SOA bucket stuff is making things too complicated
Let's just put the 2 vdoms one after the other in memory as big SOAs
See how much we need and aim for some amount of margin (tunable const)
If they run out of room, just drop it, get more system memory and retry `view`.
Avoiding that dropped data is an over-optimisation at this stage.

If current vdom is the lower one and we run into the bottom of the vdom region:
  get more memory
  move the old vdom to the new top
  drop the current vdom and regenerate it at the middle from scratch

If current vdom is the upper one and we run into the lower vdom:
  leave the old vdom where it is for now (next one will go in middle)
  drop the current vdom and regenerate it at the top from scratch

Problem:
-------
It's not just _one_ big SOA per vdom, it's a few
- one for nodes
- one for facts
- one for node-node mappings
- one for node-fact mappings
- one for node-extra mappings

So one way to solve this is with array-lists / bucket arrays
Another is with growable arrays...
- you can remember the sizes from last time and init to that
- when you grow, you prob need to copy and move which is pants if you have inbound pointers, so you really want to use only indices

Maybe I want a higher level language, can I use Elm?

What are the actual problems with bucket arrays?
Really it's: How do I work with pointers?
I need to get a fact or node pointer and somehow convert it to a bucket index and item index
- check if address range matches current bucket
- if not, search from the head of the bucket list until a range matches


This is just a showstopper for SOA to be honest. I just can't work with indices.

Also SOA just doesn't suit this use-case.
In the typical case where old and new vdom match, I have to load all fields anyway.

*/


void* eval_VirtualDom_node(void* args[]) {
  ElmString16* tag = args[0];
  Cons* factList = args[1];
  Cons* kidList = args[2];
  for (; factList != pNil; factList = factList->tail) {
    // what value does this fact pointer actually have? 
    // it probably should be a real pointer because otherwise confusingness
    // but it's an SOA so which addr? addr of the first field I spose
    // Then we need a function to reverse-map that to an index? at some point
    /*
    For facts, do we want to infer type from which array it's in?
    Not really cos that makes stuff harder, we have to look up the pointer location
    just to figure out the type. I'd rather dereference. We're not looping by type.
    */
    // wherever it points to, we shove it into the yoke anyways
    // they might have been created out of order though... so we really need another layer here.
  }
}


/*
Could have AOSOA...
The bucketing is at the top array level. Each SOA is a bucket.

Bucket body sizing:
How big is 1024 nodes?

1024 * 4
1024 * 8
1024 * 8
1024 * 4
= 1024 * 24
*/

void main() {

}
