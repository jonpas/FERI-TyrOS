// Interface for manipulating ordered arrays

#pragma once

#include "common.h"

// Insertion sorted array, always remains in a sorted state (between calls)
// Can store anything that can be cast to a 'void*' (uint or any pointer)
typedef void* type_t;

// Predicate should return non-zero if the first argument is less than the second, else it should return zero
typedef char (*lessthan_predicate_t)(type_t,type_t);

typedef struct {
    type_t *array;
    uint size;
    uint max_size;
    lessthan_predicate_t less_than;
} ordered_array_t;

// Standard less than predicate
char standard_lessthan_predicate(type_t a, type_t b);

// Creates an ordered array
ordered_array_t create_ordered_array(uint max_size, lessthan_predicate_t less_than);

// Places an ordered array
ordered_array_t place_ordered_array(void *addr, uint max_size, lessthan_predicate_t less_than);

// Destroys an ordered array
void destroy_ordered_array(ordered_array_t *array);

// Inserts an item into array
void insert_ordered_array(type_t item, ordered_array_t *array);

// Looks up item at index i
type_t lookup_ordered_array(uint i, ordered_array_t *array);

// Deletes item at location i from array
void remove_ordered_array(uint i, ordered_array_t *array);

