#ifndef BIT_ARRAY_H
#define BIT_ARRAY_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Included once per new array type
#define BIT_ARRAY_NAME bit_array
#define BIT_ARRAY_IMPL bit_array_impl

// Included once per new array type
#define ARRAY_NAME BIT_ARRAY_IMPL
#define ARRAY_TYPE unsigned char
#include "array/array.h"
#undef ARRAY_TYPE
#undef ARRAY_NAME

#include "bit_array/bit_array_base.h"

#undef BIT_ARRAY_NAME

#endif // BIT_ARRAY_H