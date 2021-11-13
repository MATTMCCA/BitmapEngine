#pragma once

// a=target variable, b=bit number to act upon 0-n 
//https://stackoverflow.com/questions/47981/how-do-you-set-clear-and-toggle-a-single-bit
#define BIT_SET(a,b)                ((a) |= (1ULL<<(b)))
#define BIT_CLEAR(a,b)              ((a) &= ~(1ULL<<(b)))
#define BIT_FLIP(a,b)               ((a) ^= (1ULL<<(b)))
#define BIT_CHECK(a,b)              (!!((a) & (1ULL<<(b))))        // '!!' to make sure this returns 0 or 1

#define BITMASK_SET(x, mask)        ((x) |= (mask))
#define BITMASK_CLEAR(x, mask)      ((x) &= (~(mask)))
#define BITMASK_FLIP(x, mask)       ((x) ^= (mask))
#define BITMASK_CHECK_ALL(x, mask)  (!(~(x) & (mask)))
#define BITMASK_CHECK_ANY(x, mask)  ((x) & (mask))