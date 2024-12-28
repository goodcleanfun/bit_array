#ifndef BIT_ARRAY_ALIGNED_H
#define BIT_ARRAY_ALIGNED_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Included once per new array type
#define BIT_ARRAY_NAME bit_array_aligned
#define BIT_ARRAY_ALIGNED
#include "bit_array_base.h"
#undef BIT_ARRAY_ALIGNED
#undef BIT_ARRAY_NAME

#endif // BIT_ARRAY_ALIGNED_H