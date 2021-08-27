#ifndef _IMPL_H
#define _IMPL_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct vec {
    uint32_t len;
    size_t capacity;
    int *data;
} vec;

typedef struct vec_iter {
    const vec *elems_owner;
    int *ptr;
} vec_iter;

uint32_t next_pow_of_two(uint32_t v) {
    uint32_t n = v;
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
}

int vec_at(vec_iter it) {
    return *it.ptr;
}

vec_iter iter_from_vec(vec *v) {
    vec_iter it;
    it.elems_owner = v;
    it.ptr = v->data;
    return it;
}

bool vec_iter_is_at_end(vec_iter it) {
    const vec *v = it.elems_owner;
    return it.ptr == v->data + v->len;
}

void vec_iter_advance(vec_iter *it) {
    it->ptr++;
}


void vec_init(vec *v) {
    v->len = 0;
    v->capacity = 0;
    v->data = NULL;
}

vec vec_from_elems(int *elems, uint32_t n) {
    vec v;
    v.len = n;
    v.capacity = next_pow_of_two(n);
    v.data = (int *) malloc(v.capacity * sizeof(int));
    for (uint32_t i = 0; i < n; i++) {
        v.data[i] = elems[i];
    }
    return v;
}

#define START_CAPACITY 8
#define RESIZE_FACTOR 2

static void vec_maybe_grow(vec *vec) {
    if (vec->capacity == 0) {
        vec->capacity = START_CAPACITY;
        vec->data = (int *) malloc(START_CAPACITY * sizeof(int));
        return;
    }
    if (vec->capacity == vec->len) {
        vec->capacity = RESIZE_FACTOR * vec->capacity;
        int *old_ptr = vec->data;
        vec->data = (int *) malloc(sizeof(int) * vec->capacity);
        memcpy(vec->data, old_ptr, (size_t) (vec->len * sizeof(int)));
        free(old_ptr);
    }
}

static void vec_grow_to(vec *vec, uint32_t new_cap) {
    if (vec->capacity == 0) {
        vec->capacity = new_cap;
        vec->data = (int *) malloc(new_cap * sizeof(int));
        return;
    }
    vec->capacity = new_cap;
    int *old_ptr = vec->data;
    vec->data = (int *) malloc(sizeof(int) * vec->capacity);
    memcpy(vec->data, old_ptr, (size_t) (vec->len * sizeof(int)));
    free(old_ptr);
}

void vec_push(vec *vec, int elem) {
    vec_maybe_grow(vec);
    vec->data[vec->len] = elem;
    vec->len++;
}

typedef void (*vec_free_fn)(int);

void vec_free(vec *v, vec_free_fn free_fn) {
    if (free_fn != NULL) {
        for (uint32_t i = 0; i < v->len; i++) {
            free_fn(v->data[i]);
        }
    }
    free(v->data);
}

void vec_push_elems(vec *vec, int *elems, uint32_t n) {
    if (n == 0 || elems == NULL) {
        return;
    }
    uint32_t required = vec->len + n; 
    if (required <= vec->capacity) {
        // We have enough space in the vector, just put everything inside
        memcpy(vec->data + vec->len, elems, (size_t) ((required - vec->len) * sizeof(int)) ); 
        vec->len = required;
        return;
    }
    uint32_t new_cap = next_pow_of_two(required);
    vec_grow_to(vec, new_cap);
    memcpy(vec->data + vec->len, elems, (size_t) ((required - vec->len) * sizeof(int)) ); 
    vec->len = required;
}

#endif 
