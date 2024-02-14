#ifndef BIT_ARRAY_H
#define BIT_ARRAY_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Included once per new array type
#define ARRAY_NAME bit_array_impl
#define ARRAY_TYPE unsigned char
#include "array/array.h"
#undef ARRAY_TYPE
#undef ARRAY_NAME

#define BITS 8

typedef bit_array_impl bit_array;

#define BYTES_SIZE(n) ((n) / BITS + ((n) % BITS != 0))

static inline bit_array *bit_array_new_size(size_t size) {
    return bit_array_impl_new_size(BYTES_SIZE(size));
}

static inline bit_array *bit_array_new(void) {
    return bit_array_impl_new();
}

static inline bit_array *bit_array_new_size_fixed(size_t size) {
    return bit_array_impl_new_size_fixed(BYTES_SIZE(size));
}

static inline bit_array *bit_array_new_aligned(size_t size, size_t alignment) {
    return bit_array_impl_new_aligned(BYTES_SIZE(size), alignment);
}

static inline bool bit_array_resize(bit_array *array, size_t size) {
    return bit_array_impl_resize(array, BYTES_SIZE(size));
}

static inline bool bit_array_resize_to_fit(bit_array *array, size_t needed_capacity) {
    return bit_array_impl_resize_to_fit(array, BYTES_SIZE(needed_capacity));
}

static inline bool bit_array_resize_aligned(bit_array *array, size_t size, size_t alignment) {
    return bit_array_impl_resize_aligned(array, BYTES_SIZE(size), alignment);
}

static inline bool bit_array_resize_fixed(bit_array *array, size_t size) {
    return bit_array_impl_resize_fixed(array, BYTES_SIZE(size));
}

static inline bool bit_array_resize_fixed_aligned(bit_array *array, size_t size, size_t alignment) {
    return bit_array_impl_resize_fixed_aligned(array, BYTES_SIZE(size), alignment);
}


static inline bool bit_array_get(bit_array *array, size_t index) {
    if (index >= array->n) return false;
    return (array->a[index / BITS] & (1 << (BITS - 1 - (index % BITS)))) > 0;
}


static inline bool bit_array_set(bit_array *array, size_t index, bool value) {
    if (index >= array->n) return false;
    if (value) {
        array->a[index / BITS] |= (1 << (BITS - 1 - (index % BITS)));
    } else {
        array->a[index / BITS] &= ~(1 << (BITS - 1 - (index % BITS)));
    }
    return true;
}

static inline bool bit_array_push(bit_array *array, bool value) {
    if (BYTES_SIZE(array->n + 1) > array->m) {
        if (!bit_array_resize_to_fit(array, array->n + 1)) return false;
    }
    if (value) {
        array->a[array->n / BITS] |= (1 << (BITS - 1 - array->n % BITS));
    } else {
        array->a[array->n / BITS] &= ~(1 << (BITS - 1 - array->n % BITS));
    }
    array->n++;
    return true;
}

static inline bool bit_array_extend(bit_array *array, bool *values, size_t n) {
    size_t new_size = array->n + n;
    size_t current_capacity = array->m;
    if (!bit_array_resize_to_fit(array, new_size)) return false;
    for (size_t i = 0; i < n; i++) {
        bit_array_push(array, values[i]);
    }
    return true;
}

static inline bool bit_array_concat(bit_array *array, bit_array *other) {
    size_t new_size = array->n + other->n;
    size_t current_capacity = array->m;
    if (!bit_array_resize_to_fit(array, new_size)) return false;
    for (size_t i = 0; i < other->n; i++) {
        bit_array_push(array, bit_array_get(other, i));
    }
    return true;
}

static inline bool bit_array_pop(bit_array *array, bool *result) {
    if (array->n == 0) return false;
    *result = bit_array_get(array, array->n - 1);
    array->n--;
    return true;
}

static inline void bit_array_clear(bit_array *array) {
    array->n = 0;
}

static inline bool bit_array_copy(bit_array *dst, bit_array *src, size_t n) {
    size_t copy_size = BYTES_SIZE(n);
    return bit_array_impl_copy(dst, src, copy_size);
}
static inline bit_array *bit_array_new_copy(bit_array *array, size_t n) {
    bit_array *cpy = bit_array_new_size(n);
    if (!bit_array_copy(cpy, array, n)) return NULL;
    return cpy;
}

static inline bit_array *bit_array_new_ones(size_t n) {
    bit_array *array = bit_array_new_size(n);
    if (array == NULL) return NULL;
    memset(array->a, 0xFF, BYTES_SIZE(n));
    if (n % BITS != 0) {
        array->a[n / BITS] = 0xFF & ~(1 << ((BITS - 1 - (n % BITS))));
    }
    array->n = n;
    return array;
}

static inline bit_array *bit_array_new_zeros(size_t n) {
    bit_array *array = bit_array_new_size(n);
    if (array == NULL) return NULL;
    memset(array->a, 0, BYTES_SIZE(n));
    array->n = n;
    return array;
}

static inline bit_array *bit_array_new_value(size_t n, bool value) {
    if (value) {
        return bit_array_new_ones(n);
    } else {
        return bit_array_new_zeros(n);
    }
}

static inline void bit_array_set_all(bit_array *array, bool value) {
    if (value) {
        memset(array->a, 0xFF, array->n / BITS);
        array->a[array->n / BITS] &= ~(1 << ((BITS - 1 - (array->n % BITS))));
    } else {
        memset(array->a, 0, BYTES_SIZE(array->n));
    }
}

static inline void bit_array_flip(bit_array *array, size_t index) {
    array->a[index / BITS] ^= (1 << (BITS - 1 - (index % BITS)));
}

static inline void bit_array_flip_all(bit_array *array) {
    for (size_t i = 0; i < BYTES_SIZE(array->n); i++) {
        array->a[i] = ~array->a[i];
    }
    if (array->n % BITS != 0) {
        array->a[array->n / BITS] &= ~(1 << ((BITS - 1 - (array->n % BITS))));
    }
}

static inline void bit_array_destroy(bit_array *array) {
    bit_array_impl_destroy(array);
}

static inline void bit_array_destroy_aligned(bit_array *array) {
    bit_array_impl_destroy_aligned(array);
}

#undef BYTES_SIZE
#undef BITS

#endif // BIT_ARRAY_H