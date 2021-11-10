/*
    ***************************************************************************************
    MJM -2021-
    zpl

    =======================================================================================

    File: zpl.hpp

        Notes: Minimal ZPL Lib
        http://labelary.com/viewer.html

    =======================================================================================

    Copyright <2021> <Matthew McCardle>
    Permission is hereby granted, free of charge, to any person obtaining a copy of
    this software and associated documentation files (the "Software"), to deal in the
    Software without restriction, including without limitation the rights to use, copy,
    modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
    and to permit persons to whom the Software is furnished to do so, subject to the
    following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
    INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
    PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
    COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
    IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
    WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

    ***************************************************************************************
*/

#pragma once

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstdbool>
#include <cstring>
#include <cmath>
#include <string>

#include "crc.hpp"


extern "C" {
#include "base64.h"
#include "miniz.h"
}

#define BUFFER_SIZE 300         //zpl header/footer string buffer
#define BUF_SIZE (1024 * 1024)  //lz77 working buffer

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

// lz77 macros
#define my_max(a,b) (((a) > (b)) ? (a) : (b))
#define my_min(a,b) (((a) < (b)) ? (a) : (b))

/* ZPL format & compression tables */
static const int _frequency[39] = { 1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,
                                   12,  13,  14,  15,  16,  17,  18,  19,
                                   20,  40,  60,  80, 100, 120, 140, 160, 180, 200, 220,
                                  240, 260, 280, 300, 320, 340, 360, 380, 400 };

static const char _enc[39] = { 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
                               'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y',
                               'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q',
                               'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };

/* ^MNa*/
typedef struct {
    char media_being_used;
    int32_t bl;
}MEDIA_TRACKING;

/* ^LHx,y */
typedef struct {
    uint32_t x;
    uint32_t y;
}LABEL_HOME;

/* ^MMa,b */
typedef struct {
    char mode;
    char prepeel_select;
}PRINT_MODE;

/* ^PWa */
typedef struct {
    uint32_t label_width;
}PRINT_WIDTH;

/* ^LSa */
typedef struct {
    int32_t shift_left_value;
}LABEL_SHIFT;

/* ^POa */
typedef struct {
    char value;
}PRINT_ORIENTATION;

/* ^LLy */
typedef struct {
    uint32_t y_axis_pos;
}LABEL_LENGTH;

/* ^PRp,s,b */
typedef struct {
    uint32_t print_speed;
    uint32_t slew_speed;
    uint32_t backfeed_speed;
}PRINT_RATE;

/* ~SD## */
typedef struct {
    uint32_t darkness_setting;
}DARKNESS;

/* ^FOx,y,z */
typedef struct {
    uint32_t x;
    uint32_t y;
    uint32_t z;
}FIELD_ORIGIN;

/* ^PQq,p,r,o,e */
typedef struct {
    uint32_t q;
    uint32_t p;
    uint32_t r;
    char o;
    char e;
}PRINT_QUANTITY;

typedef struct {
    MEDIA_TRACKING media_tracking;
    LABEL_HOME lable_home;
    PRINT_MODE print_mode;
    PRINT_WIDTH print_width;
    LABEL_SHIFT label_shift;
    PRINT_ORIENTATION print_orientation;
    LABEL_LENGTH label_length;
    PRINT_RATE print_rate;
    DARKNESS darkness;
    FIELD_ORIGIN field_origin;
    PRINT_QUANTITY print_quantity;
}PARM;

class zpl
{
public:
    zpl();
    zpl(PARM init);

    bool set_media_tracking(char a, int32_t b);                                         /* ^MNa,b*/
    bool get_media_tracking(char* a, int32_t* b);                                       /* ^MNa,b*/
    bool set_label_home(uint32_t x0, uint32_t y0);                                      /* ^LHx,y */
    bool get_label_home(uint32_t* x0, uint32_t* y0);                                    /* ^LHx,y */
    bool set_print_mode(char a, char b);                                                /* ^MMa,b */
    bool get_print_mode(char* a, char* b);                                              /* ^MMa,b */
    bool set_print_width(uint32_t y0);                                                  /* ^PWa */
    bool get_print_width(uint32_t* y0);                                                 /* ^PWa */
    bool set_label_shift(int32_t y0);                                                   /* ^LSa */
    bool get_label_shift(int32_t* y0);                                                  /* ^LSa */
    bool set_print_orientation(char a);                                                 /* ^POa */
    bool get_print_orientation(char* a);                                                /* ^POa */
    bool set_label_length(uint32_t y0);                                                 /* ^LLy */
    bool get_label_length(uint32_t* y0);                                                /* ^LLy */
    bool set_print_rate(uint32_t p, uint32_t s, uint32_t b);                            /* ^PRp,s,b */
    bool get_print_rate(uint32_t* p, uint32_t* s, uint32_t* b);                         /* ^PRp,s,b */
    bool set_darkness(uint32_t d);                                                      /* ~SD## */
    bool get_darkness(uint32_t* d);                                                     /* ~SD## */
    bool set_field_origin(uint32_t x0, uint32_t y0, uint32_t z0);                       /* ^FOx,y,z */
    bool get_field_origin(uint32_t* x0, uint32_t* y0, uint32_t* z0);                    /* ^FOx,y,z */
    bool set_print_quantity(uint32_t q, uint32_t p, uint32_t r, char o, char e);        /* ^PQq,p,r,o,e */
    bool get_print_quantity(uint32_t* q, uint32_t* p, uint32_t* r, char* o, char* e);   /* ^PQq,p,r,o,e */
    bool add_graphic(bool* ptr, uint32_t x0, int32_t y0);                               /* ^GFA,b,c,d,data */
    bool save_format(const char* fileName);
    bool get_format(uint8_t** ptr, uint32_t* len);
    bool generate_format(void);

    ~zpl();

private:

    //const char* _freq_to_string(char val, int32_t freq);
    //bool _bytes_to_zpl(void);
    bool _pack_bool(bool* ptr, uint32_t _size, uint32_t x0);
    bool _gen_zpl(void);
    bool _gen_head(void);
    bool _gen_foot(void);
    bool _GFA_prefix(int32_t y0);


    bool _compress(uint8_t** ptr, uint32_t *len);
    bool _encode(uint8_t* ptr, uint32_t len);



    void _free_int_buf(void) {
        if (s_inbuf != nullptr)  delete[] s_inbuf;
        if (s_outbuf != nullptr) delete[] s_outbuf;
        s_inbuf = nullptr;
        s_outbuf = nullptr;
    };

    bool _alloc_int_buf(void) {
        if (s_inbuf == nullptr)  s_inbuf = new uint8_t[BUF_SIZE];
        if (s_outbuf == nullptr) s_outbuf = new uint8_t[BUF_SIZE];            
        if (s_inbuf == nullptr || s_outbuf == nullptr)
            return 1;
        return 0;
    };


protected:

    PARM job = {0x00};
    uint8_t* zpl_data = nullptr;
    uint32_t zpl_data_size = 0;
    uint32_t zpl_row = 0;

    uint8_t* _zpl = nullptr;
    uint32_t _zpl_size = 0;

    /* TODO: make dynamic? */
    char HEAD[BUFFER_SIZE] = { 0x00 };
    char FOOT[BUFFER_SIZE] = { 0x00 };

    uint8_t *s_inbuf = nullptr;
    uint8_t *s_outbuf = nullptr;

};