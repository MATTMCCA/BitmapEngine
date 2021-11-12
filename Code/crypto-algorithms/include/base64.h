/*********************************************************************
* Filename:   base64.h
* Author:     Brad Conte (brad AT bradconte.com)
* Copyright:
* Disclaimer: This code is presented "as is" without any guarantees.
* Details:    Defines the API for the corresponding Base64 implementation.
*********************************************************************/

#ifndef BASE64_H
#define BASE64_H

/*************************** HEADER FILES ***************************/
#include <stddef.h>
#include <stdint.h>

/**************************** DATA TYPES ****************************/
//typedef uint8_t BYTE;             // 8-bit byte

/*********************** FUNCTION DECLARATIONS **********************/
// Returns the size of the output. If called with out = NULL, will just return
// the size of what the output would have been (without a terminating NULL).
uint32_t base64_encode(uint8_t *in, uint8_t *out, uint32_t len, int newline_flag);

// Returns the size of the output. If called with out = NULL, will just return
// the size of what the output would have been (without a terminating NULL).
uint32_t base64_decode(uint8_t *in, uint8_t *out, uint32_t len);

#endif   // BASE64_H
