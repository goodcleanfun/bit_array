#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef BIT_ARRAY_NAME
#error "Must define BIT_ARRAY_NAME"
#endif

#define BIT_ARRAY_CONCAT_(a, b) a ## b
#define BIT_ARRAY_CONCAT(a, b) BIT_ARRAY_CONCAT_(a, b)
#define BIT_ARRAY_FUNC(name) BIT_ARRAY_CONCAT(BIT_ARRAY_NAME, _##name)
#define BIT_ARRAY_IMPL BIT_ARRAY_CONCAT(BIT_ARRAY_NAME, _impl)
#define BIT_ARRAY_IMPL_FUNC(name) BIT_ARRAY_CONCAT(BIT_ARRAY_IMPL, _##name)

// Included once per new array type
#define ARRAY_NAME BIT_ARRAY_IMPL
#define ARRAY_TYPE unsigned char
#ifndef BIT_ARRAY_ALIGNED
#include "array/array.h"
#else
#include "aligned/aligned_array.h"
#endif
#undef ARRAY_TYPE
#undef ARRAY_NAME

typedef BIT_ARRAY_IMPL BIT_ARRAY_NAME;

#define BITS 8

#define BYTES_SIZE(n) ((n) / BITS + ((n) % BITS != 0))

static inline BIT_ARRAY_NAME *BIT_ARRAY_FUNC(new_size)(size_t size) {
    return BIT_ARRAY_IMPL_FUNC(new_size)(BYTES_SIZE(size));
}

static inline BIT_ARRAY_NAME *BIT_ARRAY_FUNC(new)(void) {
    return BIT_ARRAY_IMPL_FUNC(new)();
}

static inline BIT_ARRAY_NAME *BIT_ARRAY_FUNC(new_size_fixed)(size_t size) {
    return BIT_ARRAY_IMPL_FUNC(new_size_fixed)(BYTES_SIZE(size));
}

static inline bool BIT_ARRAY_FUNC(resize)(BIT_ARRAY_NAME *array, size_t size) {
    return BIT_ARRAY_IMPL_FUNC(resize)(array, BYTES_SIZE(size));
}

static inline bool BIT_ARRAY_FUNC(resize_to_fit)(BIT_ARRAY_NAME *array, size_t needed_capacity) {
    return BIT_ARRAY_IMPL_FUNC(resize_to_fit)(array, BYTES_SIZE(needed_capacity));
}


static inline bool BIT_ARRAY_FUNC(resize_fixed)(BIT_ARRAY_NAME *array, size_t size) {
    return BIT_ARRAY_IMPL_FUNC(resize_fixed)(array, BYTES_SIZE(size));
}

static inline bool BIT_ARRAY_FUNC(get)(BIT_ARRAY_NAME *array, size_t index) {
    if (index >= array->n) return false;
    return (array->a[index / BITS] & (1 << (BITS - 1 - (index % BITS)))) > 0;
}


static inline bool BIT_ARRAY_FUNC(set)(BIT_ARRAY_NAME *array, size_t index, bool value) {
    if (index >= array->n) return false;
    if (value) {
        array->a[index / BITS] |= (1 << (BITS - 1 - (index % BITS)));
    } else {
        array->a[index / BITS] &= ~(1 << (BITS - 1 - (index % BITS)));
    }
    return true;
}

static inline bool BIT_ARRAY_FUNC(push)(BIT_ARRAY_NAME *array, bool value) {
    if (BYTES_SIZE(array->n + 1) > array->m) {
        if (!BIT_ARRAY_FUNC(resize_to_fit)(array, array->n + 1)) return false;
    }
    if (value) {
        array->a[array->n / BITS] |= (1 << (BITS - 1 - array->n % BITS));
    } else {
        array->a[array->n / BITS] &= ~(1 << (BITS - 1 - array->n % BITS));
    }
    array->n++;
    return true;
}

static inline bool BIT_ARRAY_FUNC(extend)(BIT_ARRAY_NAME *array, bool *values, size_t n) {
    size_t new_size = array->n + n;
    size_t current_capacity = array->m;
    if (!BIT_ARRAY_FUNC(resize_to_fit)(array, new_size)) return false;
    for (size_t i = 0; i < n; i++) {
        BIT_ARRAY_FUNC(push)(array, values[i]);
    }
    return true;
}

static inline bool BIT_ARRAY_FUNC(concat)(BIT_ARRAY_NAME *array, BIT_ARRAY_NAME *other) {
    size_t new_size = array->n + other->n;
    size_t current_capacity = array->m;
    if (!BIT_ARRAY_FUNC(resize_to_fit)(array, new_size)) return false;
    for (size_t i = 0; i < other->n; i++) {
        BIT_ARRAY_FUNC(push)(array, BIT_ARRAY_FUNC(get)(other, i));
    }
    return true;
}

static inline bool BIT_ARRAY_FUNC(pop)(BIT_ARRAY_NAME *array, bool *result) {
    if (array->n == 0) return false;
    *result = BIT_ARRAY_FUNC(get)(array, array->n - 1);
    array->n--;
    return true;
}

static inline void BIT_ARRAY_FUNC(clear)(BIT_ARRAY_NAME *array) {
    array->n = 0;
}

static inline bool BIT_ARRAY_FUNC(copy)(BIT_ARRAY_NAME *dst, BIT_ARRAY_NAME *src, size_t n) {
    size_t copy_size = BYTES_SIZE(n);
    return BIT_ARRAY_IMPL_FUNC(copy)(dst, src, copy_size);
}
static inline BIT_ARRAY_NAME *BIT_ARRAY_FUNC(new_copy)(BIT_ARRAY_NAME *array, size_t n) {
    BIT_ARRAY_NAME *cpy = BIT_ARRAY_FUNC(new_size)(n);
    if (!BIT_ARRAY_FUNC(copy)(cpy, array, n)) return NULL;
    return cpy;
}

static inline BIT_ARRAY_NAME *BIT_ARRAY_FUNC(new_ones)(size_t n) {
    BIT_ARRAY_NAME *array = BIT_ARRAY_FUNC(new_size)(n);
    if (array == NULL) return NULL;
    memset(array->a, 0xFF, BYTES_SIZE(n));
    if (n % BITS != 0) {
        array->a[n / BITS] = 0xFF & ~(1 << ((BITS - 1 - (n % BITS))));
    }
    array->n = n;
    return array;
}

static inline BIT_ARRAY_NAME *BIT_ARRAY_FUNC(new_zeros)(size_t n) {
    BIT_ARRAY_NAME *array = BIT_ARRAY_FUNC(new_size)(n);
    if (array == NULL) return NULL;
    memset(array->a, 0, BYTES_SIZE(n));
    array->n = n;
    return array;
}

static inline BIT_ARRAY_NAME *BIT_ARRAY_FUNC(new_value)(size_t n, bool value) {
    if (value) {
        return BIT_ARRAY_FUNC(new_ones)(n);
    } else {
        return BIT_ARRAY_FUNC(new_zeros)(n);
    }
}

static inline void BIT_ARRAY_FUNC(set_all)(BIT_ARRAY_NAME *array, bool value) {
    if (value) {
        memset(array->a, 0xFF, array->n / BITS);
        array->a[array->n / BITS] &= ~(1 << ((BITS - 1 - (array->n % BITS))));
    } else {
        memset(array->a, 0, BYTES_SIZE(array->n));
    }
}

static inline void BIT_ARRAY_FUNC(flip)(BIT_ARRAY_NAME *array, size_t index) {
    array->a[index / BITS] ^= (1 << (BITS - 1 - (index % BITS)));
}

static inline void BIT_ARRAY_FUNC(flip_all)(BIT_ARRAY_NAME *array) {
    for (size_t i = 0; i < BYTES_SIZE(array->n); i++) {
        array->a[i] = ~array->a[i];
    }
    if (array->n % BITS != 0) {
        array->a[array->n / BITS] &= ~(1 << ((BITS - 1 - (array->n % BITS))));
    }
}

static inline void BIT_ARRAY_FUNC(destroy)(BIT_ARRAY_NAME *array) {
    BIT_ARRAY_IMPL_FUNC(destroy)(array);
}

#undef BYTES_SIZE
#undef BITS
#undef BIT_ARRAY_CONCAT
#undef BIT_ARRAY_CONCAT_
#undef BIT_ARRAY_FUNC
#undef BIT_ARRAY_IMPL
#undef BIT_ARRAY_IMPL_FUNC
