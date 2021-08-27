#ifndef _VEC_H
#define _VEC_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef struct S {
    int x;
    int y;
    bool why;
    char *str;
} something;

typedef void * opaque_handle;

static uint32_t next_pow_of_two(uint32_t); 

#define vec_ty(__ty) vec_ ## __ty
#define vec_iter(__ty) vec_ ## __ty ## _iter

#define START_CAPACITY 8
#define RESIZE_FACTOR 2


#define vec_define(__ty) \
    typedef struct vec_ty(__ty) { \
       size_t len;                        \
       size_t capacity;                        \
       __ty *data;                        \
    } vec_ty(__ty) ; \
    typedef struct vec_iter(__ty) { \
        const vec_ty(__ty) *elems_owner; \
        __ty *ptr; \
        } vec_iter(__ty);  \
    __ty vec_ ## __ty ##_at (vec_iter(__ty) it) { \
        return *it.ptr; \
    } \
    vec_iter(__ty) iter_from_vec_ ## __ty(vec_ty(__ty) *v) { \
        vec_iter(__ty) it; \
        it.elems_owner = v; \
        it.ptr = v->data; \
        return it; \
    } \
    bool vec_ ## __ty ## _iter_is_at_end(vec_iter(__ty) it) { \
        const vec_ty(__ty) *v = it.elems_owner; \
        return it.ptr == v->data + v->len; \
    } \
    void vec_ ## __ty ## _iter_advance(vec_iter(__ty) *it) { \
        it->ptr++; \
    } \
    void vec_ ## __ty ## _init(vec_ty(__ty) *v) { \
        v->len = 0; \
        v->capacity = 0; \
        v->data = NULL; \
    } \
    vec_ty(__ty) vec_ ## __ty ## _from_elems(__ty *elems, uint32_t n) { \
        vec_ty(__ty) v; \
        v.len = n; \
        v.capacity = next_pow_of_two(n); \
        v.data = (__ty *) malloc(v.capacity * sizeof(__ty)); \
        for (uint32_t i = 0; i < n; i++) { \
            v.data[i] = elems[i]; \
        } \
        return v; \
    } \
    static void vec_ ##__ty ## _maybe_grow(vec_ty(__ty) *vec) { \
        if (vec->capacity == 0) { \
            vec->capacity = START_CAPACITY; \
            vec->data = (__ty *) malloc(START_CAPACITY * sizeof(__ty)); \
            return; \
        } \
        if (vec->capacity == vec->len) { \
            vec->capacity = RESIZE_FACTOR * vec->capacity; \
            __ty *old_ptr = vec->data; \
            vec->data = (__ty *) malloc(sizeof(__ty) * vec->capacity); \
            memcpy(vec->data, old_ptr, (size_t) (vec->len * sizeof(__ty))); \
            free(old_ptr); \
        } \
    } \
    static void vec_ ## __ty ## _grow_to(vec_ty(__ty) *vec, uint32_t new_cap) { \
        if (vec->capacity == 0) { \
            vec->capacity = new_cap; \
            vec->data = (__ty *) malloc(new_cap * sizeof(__ty)); \
            return; \
        } \
        vec->capacity = new_cap; \
        __ty *old_ptr = vec->data; \
        vec->data = (__ty *) malloc(sizeof(__ty) * vec->capacity); \
        memcpy(vec->data, old_ptr, (size_t) (vec->len * sizeof(__ty))); \
        free(old_ptr); \
    } \
    void vec_ ## __ty ## _push(vec_ty(__ty) *vec, __ty elem) { \
        vec_ ## __ty ## _maybe_grow(vec); \
        vec->data[vec->len] = elem; \
        vec->len++; \
    } \
    typedef void (*vec_ ## __ty ## _free_fn)(__ty); \
    void vec_ ## __ty ## _free(vec_ty(__ty) *v, vec_ ## __ty ## _free_fn free_fn) { \
        if (free_fn != NULL) { \
            for (uint32_t i = 0; i < v->len; i++) { \
                free_fn(v->data[i]); \
            } \
        } \
        free(v->data); \
    } \
    void vec_ ## __ty ## _push_elems(vec_ty(__ty) *vec, __ty *elems, uint32_t n) { \
        if (n == 0 || elems == NULL) { \
            return; \
        } \
        uint32_t required = vec->len + n; \
        if (required <= vec->capacity) { \
            memcpy(vec->data + vec->len, elems, (size_t) ((required - vec->len) * sizeof(__ty)) );  \
            vec->len = required; \
            return; \
        } \
        uint32_t new_cap = next_pow_of_two(required); \
        vec_ ## __ty ## _grow_to(vec, new_cap); \
        memcpy(vec->data + vec->len, elems, (size_t) ((required - vec->len) * sizeof(__ty)) );  \
        vec->len = required; \
    }



vec_define(something)
vec_define(opaque_handle)

#endif

