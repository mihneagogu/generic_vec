#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "vec.h"
#include "impl_example.h"

static void free_int(int i) {
    printf("Freeing %d int\n", i);
}

void free_something(something s) {
    if (s.str) {
        printf("freed a something %s!\n", s.str);
        free(s.str);
    }
}

int main(void) {
    vec_something vs;
    vec_something_init(&vs);
    for (int i = 0; i < 100; i++) {
        something s;
        if (i % 10 == 0) {
            s.str = strdup("hellooo");
        } else {
            s.str = NULL;
        }
        vec_something_push(&vs, s);
    }
    vec_something_free(&vs, free_something);

    vec_something v2;
    vec_something_init(&v2);

    const uint32_t test_size = 129;

    something *arr = malloc(test_size * sizeof(something));
    for (int i = 0; i < 129; i++) {
       something sth; 
       if (i % 4 == 0) {
        sth.str = strdup("byebye");
       } else {
        sth.str = NULL;
       }
       arr[i] = sth;
    }

    vec_something_push_elems(&v2, arr, test_size);
    free(arr);
    vec_something_free(&v2, free_something);

    return EXIT_SUCCESS;
}
