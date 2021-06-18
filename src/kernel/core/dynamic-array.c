#include "./core.h"

DynamicArray* newDynamicArray(u32 capacity) {
  size_t size = (sizeof(DynamicArray) / SIZE_UNIT) + capacity;
  DynamicArray* a = GC_allocate(true, size);
  a->header = (Header){.size = size, .tag = Tag_Custom};
  a->occupied = 0;
  for (size_t i = 0; i < capacity; i++) {
    a->values[i] = NULL;
  }
  return a;
}

// Push a value onto the end of the array
// Side-effect: re-allocates the array if needed
// Returned index is temporary. Can change if another value is removed
void DynamicArray_push(DynamicArray** array_ref, void* value) {
  DynamicArray* orig = *array_ref;
  DynamicArray* current;
  u32 orig_capacity = custom_params((Custom*)orig);
  if (orig->occupied < orig_capacity) {
    current = orig;
  } else {
    size_t new_capacity = (orig_capacity < 1024) ? (orig_capacity * 2) : (orig_capacity + 1024);
    current = newDynamicArray(new_capacity);
    for (size_t i = 0; i < orig_capacity; i++) {
      current->values[i] = orig->values[i];
    }
    for (size_t i = orig_capacity; i < new_capacity; i++) {
      current->values[i] = NULL;
    }
  }
  u32 index = current->occupied;
  current->values[index] = value;
  current->occupied++;
  *array_ref = current;
}

// Remove a value from the array
// Side-effect: reorders the values so the last slot is free
void DynamicArray_remove(DynamicArray* array, u32 index) {
  u32 last = array->occupied - 1;
  array->values[index] = array->values[last];
  array->values[last] = NULL;
  array->occupied--;
}
