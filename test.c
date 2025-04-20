#include <stdint.h>
#include "greatest/greatest.h"

#define DEFAULT_ARRAY_SIZE 8
#include "bit_array/bit_array.h"

TEST test_bit_array(void) {
    bit_array *v = bit_array_new();
    ASSERT_EQ(v->m, DEFAULT_ARRAY_SIZE);
    ASSERT_EQ(v->n, 0);

    for (int32_t i = 0; i < 80; i++) {
        bit_array_push(v, i % 2);
    }
    size_t expected_size = (DEFAULT_ARRAY_SIZE * 3 / 2);
    ASSERT_EQ(v->m, expected_size);
    ASSERT_EQ(v->n, 80);

    for (int32_t i = 0; i < 10; i++) {
        ASSERT_EQ(v->a[i], 0x55);
    }

    for (int32_t i = 0; i < 80; i++) {
        ASSERT_EQ(bit_array_get(v, i), i % 2);
    }

    bit_array *w = bit_array_new_size(128);
    ASSERT_EQ(w->m, 16);
    ASSERT_EQ(w->n, 0);

    for (int32_t i = 0; i < 136; i++) {
        bit_array_push(w, i % 2);
    }
    ASSERT_EQ(w->m, 16 * 3 / 2);
    ASSERT_EQ(w->n, 136);

    bit_array_concat(v, w);
    expected_size = expected_size * 3 / 2 * 3 / 2;
    ASSERT_EQ(v->m, expected_size);
    ASSERT_EQ(v->n, 216);
    size_t current_cap = v->m;
    ASSERT(bit_array_resize_to_fit(v, current_cap));
    ASSERT_EQ(v->m, current_cap);

    bit_array_extend(v, (bool[]){0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0}, 24);
    ASSERT_EQ(v->n, 240);
    ASSERT_EQ(bit_array_get(v, 238), 1);
    ASSERT_EQ(bit_array_get(v, 239), 0);

    ASSERT(bit_array_set(v, 238, 0));
    ASSERT(bit_array_set(v, 239, 1));
    ASSERT_EQ(bit_array_get(v, 238), 0);
    ASSERT_EQ(bit_array_get(v, 239), 1);

    bit_array_flip(v, 238);
    bit_array_flip(v, 239);
    ASSERT_EQ(bit_array_get(v, 238), 1);
    ASSERT_EQ(bit_array_get(v, 239), 0);

    bool val;
    ASSERT(bit_array_pop(v, &val));
    ASSERT_EQ(val, 0);
    ASSERT_EQ(v->n, 239);

    bit_array_flip_all(v);
    ASSERT_EQ(bit_array_get(v, 0), 1);
    ASSERT_EQ(bit_array_get(v, 238), 0);

    current_cap = v->m;
    ASSERT(bit_array_resize_to_fit(v, (current_cap * 3 / 2) - 1));
    expected_size = expected_size * 3 / 2;
    ASSERT_EQ(v->m, expected_size);

    bit_array *x = bit_array_new_ones(127);
    ASSERT(x != NULL);
    ASSERT_EQ(x->m, 16);
    ASSERT_EQ(x->n, 127);
    for (int32_t i = 0; i < 127; i++) {
        ASSERT_EQ(bit_array_get(x, i), 1);
    }

    bit_array *y = bit_array_new_zeros(127);
    ASSERT_EQ(y->m, 16);
    ASSERT_EQ(y->n, 127);
    for (int32_t i = 0; i < 127; i++) {
        ASSERT_EQ(bit_array_get(y, i), 0);
    }

    bit_array_destroy(x);
    bit_array_destroy(y);
    bit_array_destroy(v);
    bit_array_destroy(w);
    PASS();
}

/* Add definitions that need to be in the test runner's main file. */
GREATEST_MAIN_DEFS();

int32_t main(int32_t argc, char **argv) {
    GREATEST_MAIN_BEGIN();      /* command-line options, initialization. */

    RUN_TEST(test_bit_array);

    GREATEST_MAIN_END();        /* display results */
}
