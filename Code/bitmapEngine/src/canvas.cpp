/*
    ***************************************************************************************
    MJM -2021-
    canvas / bitmap Lib
    
    ======================================================================================= 
    
    File: canvas.cpp
    
        Notes: Very little error handling. Like for real, this needs looked at sometime.
        
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

#include "canvas.hpp"

canvas::canvas()
{

}

bool canvas::create(int32_t x, int32_t y, bool invert)
{
    if (ptr == nullptr) {
        _x = x;
        _y = y;
        _inv = invert;
        uint32_t q = (_x * _y);
        size_t _asize = q * sizeof(bool);
        ptr = new bool[_asize] {0};
        if (ptr == nullptr)
            return 1;
    }
    return 0;
}

bool canvas::create(bool* raw, int32_t x, int32_t y, bool invert)
{
    if (!create(x, y, invert)) {
        if (ptr != nullptr) {
            uint32_t q = (_x * _y);
            memcpy(ptr, raw, q * sizeof(bool));
            if (ptr == nullptr)
                return 1;
            return 0;
        }
        return 1;
    }
    return 1;
}

bool canvas::getSize(int32_t* x, int32_t* y)
{
    if (ptr != nullptr) {
        *x = _x;
        *y = _y;
        return 0;
    }
    return 1;
}

bool canvas::fill(bool val)
{
    if (ptr != nullptr) {
        uint32_t q = (_x * _y);
        size_t _asize = q * sizeof(bool);
        memset(ptr, val, _asize);
        if (ptr == nullptr)
            return 1;
        return 0;
    }
    return 1;
}

int32_t canvas::get_y(void)
{
    return _y;
}

int32_t canvas::get_x(void)
{
    return _x;
}

bool canvas::addSprite(canvas* src, int32_t x, int32_t y, bool alpha)
{
    if ((src->ptr != nullptr) && (ptr != nullptr)) {
        uint32_t __y = 0, __x = 0;
        for (__y = 0; __y < src->_y; __y++)
            for (__x = 0; __x < src->_x; __x++)
                if (alpha) {
                    if (src->getPixle(__x, __y) ^ src->getInvert())
                        setPixle(x + __x, y + __y, 1);
                } else {
                    setPixle(x + __x, y + __y, src->getPixle(__x, __y) ^ src->getInvert());
                }
    
        return 0;
    }
    return 1;
}

/*
 * Callback procedure which is used by JBIG encoder to deliver the
 * encoded data. It simply sends the bytes to the output file.
 */
static void data_out(unsigned char* start, size_t len, void* file)
{
    fwrite(start, len, sizeof(uint8_t), (FILE*)file);
    return;
}

bool canvas::saveJBG(const char* fileName)
{
    if (ptr != nullptr) {
        uint32_t image_y = _y;
        uint32_t image_x = (uint32_t)(_x / 8.0);

        if ((image_x * 8) < (uint32_t)_x)
            image_x++;   //math fix

        if (image_x == 0)
            image_x = 1; //non 0 byte width

        uint8_t* BMPDATA = nullptr;
        uint32_t q = image_x * image_y;
        size_t BMPDATASIZE = q * sizeof(uint8_t);
        BMPDATA = new uint8_t[BMPDATASIZE]{ 0x00 };

        if (BMPDATA != nullptr)
        {
            memset(BMPDATA, 0x00, BMPDATASIZE);

            bool* in_line;
            uint8_t* out_line;
            uint32_t __y = 0, __x = 0;

            image_y--;
            for (__y = 0; __y < _y; __y++) {
                in_line = &ptr[(__y * _x)];
                out_line = &BMPDATA[(__y * image_x)];

                for (__x = 0; __x < _x; __x++) {
                    if (in_line[__x] ^ _inv)
                        BIT_SET(out_line[(__x / 8)], (7 - (__x % 8)));
                }
            }

            struct jbg85_enc_state s;

            size_t bpl = image_x;
            uint32_t width = _x, height = _y, y;      

            FILE* fd;
            fd = fopen(fileName, "wb");
            if (fd != NULL) 
            {                
                jbg85_enc_init(&s, _x, _y, data_out, fd);
                jbg85_enc_options(&s, JBG_TPBON, 0, -1);

                unsigned char *next_line = nullptr, *prev_line = nullptr, *prevprev_line = nullptr;

                for (y = 0; y < _y; y++) {

                    /* Use a 3-line ring buffer, because the encoder requires that the two
                     * previously supplied lines are still in memory when the next line is
                     * processed. */
                    next_line = &BMPDATA[y * image_x];

                    /* JBIG compress another line and write out result via callback */
                    jbg85_enc_lineout(&s, next_line, prev_line, prevprev_line);
                    prevprev_line = prev_line;
                    prev_line = next_line;

                    /* adjust final image height via NEWLEN */
                    if (y == _y)
                        jbg85_enc_newlen(&s, _y);
                }
                fclose(fd);
            }

            delete[] BMPDATA; //release when done
            return 0;
        }
        return 1;
    }
    return 1;
}

bool canvas::savePBM(const char* fileName)
{
    if (ptr != nullptr) {
        uint32_t image_y = _y;
        uint32_t image_x = (uint32_t)(_x / 8.0);

        if ((image_x * 8) < (uint32_t)_x)
            image_x++;   //math fix

        if (image_x == 0)
            image_x = 1; //non 0 byte width

        uint8_t* BMPDATA = nullptr;
        uint32_t q = image_x * image_y;
        size_t BMPDATASIZE = q * sizeof(uint8_t);
        BMPDATA = new uint8_t[BMPDATASIZE]{ 0x00 };

        if (BMPDATA != nullptr) 
        {
            memset(BMPDATA, 0x00, BMPDATASIZE);

            bool* in_line;
            uint8_t* out_line;
            uint32_t __y = 0, __x = 0;

            image_y--;
            for (__y = 0; __y < _y; __y++) {
                in_line = &ptr[(__y * _x)];
                out_line = &BMPDATA[(__y * image_x)];

                for (__x = 0; __x < _x; __x++) {
                    if (in_line[__x] ^ _inv)
                        BIT_SET(out_line[(__x / 8)], (7 - (__x % 8)));
                }
            }

            FILE* fd;
            fd = fopen(fileName, "wb");

            if (fd != NULL) {
                //TODO: add error handling, or not...
                /////////////////////////////////////////////////////////////////////////
                fprintf(fd, "P4\n# Created by BitmapEngine\n%d %d\n", _x, _y);
                /////////////////////////////////////////////////////////////////////////
                fwrite(BMPDATA, sizeof(uint8_t), BMPDATASIZE, fd);
                /////////////////////////////////////////////////////////////////////////
                fclose(fd);
            }

            delete[] BMPDATA; //release when done
            return 0;
        }
        return 1;
    }
    return 1;
}

bool canvas::saveBMP(const char* fileName, int DPI)
{
    if (ptr != nullptr) {
        uint32_t image_y = _y;
        uint32_t image_x = (uint32_t)(_x / 8.0);

        if ((image_x * 8) < (uint32_t)_x)
            image_x++;   //math fix

        if (image_x == 0)
            image_x = 1; //non 0 byte width

        while ((image_x % 4))
            image_x++;   //4 byte padding

        header bmpHead;
        bmpHead.Signature = 0x4D42;

        bmpHead.FileSize = 54 /*header*/ + 8 /*color table*/ + (image_x * image_y) /*bytes*/;

        bmpHead.reserved = 0x00;
        bmpHead.DataOffset = 62;

        infoHeader bmpInfoHead;
        bmpInfoHead.Size = 40;
        bmpInfoHead.Width = _x;
        bmpInfoHead.Height = _y;
        bmpInfoHead.Planes = 0x01;
        bmpInfoHead.Bits_Per_Pixel = 0x01;
        bmpInfoHead.Compression = 0x00;
        bmpInfoHead.ImageSize = image_x * image_y;
        bmpInfoHead.XpixelsPerM = (uint32_t)(DPI * 39.3701);
        bmpInfoHead.YpixelsPerM = (uint32_t)(DPI * 39.3701);
        bmpInfoHead.Colors_Used = 0x00;
        bmpInfoHead.Important_Colors = 0x00;

        uint8_t* BMPDATA = nullptr;
        uint32_t q = image_x * image_y;
        size_t BMPDATASIZE = q * sizeof(uint8_t);
        BMPDATA = new uint8_t[BMPDATASIZE]{ 0x00 };

        if (BMPDATA != nullptr) {
            memset(BMPDATA, 0xFF, BMPDATASIZE);

            bool* in_line;
            uint8_t* out_line;
            uint32_t __y = 0, __x = 0;

            image_y--;
            for (__y = 0; __y < _y; __y++) {
                in_line = &ptr[(__y * _x)];
                out_line = &BMPDATA[(((image_y)-__y) * image_x)];

                for (__x = 0; __x < _x; __x++) {
                    if (in_line[__x] ^ _inv)
                        BIT_CLEAR(out_line[(__x / 8)], (7 - (__x % 8)));
                }
            }

            FILE* fd;
            fd = fopen(fileName, "wb");

            if (fd != NULL) {
                //TODO: add error handling, or not...
                /////////////////////////////////////////////////////////////////////////
                fwrite(&bmpHead.Signature, sizeof(uint16_t), 1, fd);
                fwrite(&bmpHead.FileSize, sizeof(uint32_t), 1, fd);
                fwrite(&bmpHead.reserved, sizeof(uint32_t), 1, fd);
                fwrite(&bmpHead.DataOffset, sizeof(uint32_t), 1, fd);
                /////////////////////////////////////////////////////////////////////////
                fwrite(&bmpInfoHead.Size, sizeof(uint32_t), 1, fd);
                fwrite(&bmpInfoHead.Width, sizeof(uint32_t), 1, fd);
                fwrite(&bmpInfoHead.Height, sizeof(uint32_t), 1, fd);
                fwrite(&bmpInfoHead.Planes, sizeof(uint16_t), 1, fd);
                fwrite(&bmpInfoHead.Bits_Per_Pixel, sizeof(uint16_t), 1, fd);
                fwrite(&bmpInfoHead.Compression, sizeof(uint32_t), 1, fd);
                fwrite(&bmpInfoHead.ImageSize, sizeof(uint32_t), 1, fd);
                fwrite(&bmpInfoHead.XpixelsPerM, sizeof(uint32_t), 1, fd);
                fwrite(&bmpInfoHead.YpixelsPerM, sizeof(uint32_t), 1, fd);
                fwrite(&bmpInfoHead.Colors_Used, sizeof(uint32_t), 1, fd);
                fwrite(&bmpInfoHead.Important_Colors, sizeof(uint32_t), 1, fd);
                /////////////////////////////////////////////////////////////////////////
                fwrite(&COLOR_TABLE, sizeof(uint32_t), 2, fd);
                /////////////////////////////////////////////////////////////////////////
                fwrite(BMPDATA, sizeof(uint8_t), BMPDATASIZE, fd);
                /////////////////////////////////////////////////////////////////////////
                fclose(fd);
            }

            delete[] BMPDATA; //release when done
            return 0;
        }
        return 1;
    }
    return 1;
}

bool canvas::invert(bool invert)
{
    _inv = invert;
    return 0;
}

/* calling function should guard nullptr */
bool canvas::getPixle(uint32_t x, uint32_t y)
{
    if (ptr == nullptr) return 1;

    if ((y < _y) && (x < _x)) 
        return ptr[(y * _x) + x];
    return 0;
}

bool canvas::getInvert(void)
{
    return _inv;
}

/* calling function should guard nullptr */
//umm passing signed into this might be bad....
bool canvas::setPixle(uint32_t x, uint32_t y, bool val)
{
    if (ptr == nullptr) return 1;

    if ((y < _y) && (x < _x))
        ptr[(y * _x) + x] = val;
    return 0;
}

canvas::~canvas()
{
    if (ptr != nullptr)
        delete[] ptr;
}

bool canvas::drawHorizontalLine(int32_t x0, int32_t y0, int32_t width, int32_t thick, bool val)
{
    if (ptr != nullptr) {
        int32_t x = 0, y = 0;
        for (x = 0; x < width; x++)
            for (y = 0; y < thick; y++)
                setPixle(x0 + x, y0 + y, val);
        return 0;
    }
    return 1;
}

bool canvas::drawVerticalLine(int32_t x0, int32_t y0, int32_t length, int32_t thick, bool val)
{
    if (ptr != nullptr) {
        int32_t x = 0, y = 0;
        for (y = 0; y < length; y++)
            for (x = 0; x < thick; x++)
                setPixle(x0 + x, y0 + y, val);
        return 0;
    }
    return 1;
}

bool canvas::drawBox(int32_t x0, int32_t y0, int32_t length, int32_t width, int32_t thick, bool val)
{
    bool err = 0;
    err |= drawHorizontalLine(x0,                 y0,                  width,          thick, val);
    err |= drawHorizontalLine(x0,                 y0 + length - thick, width,          thick, val);
    err |= drawVerticalLine  (x0,                 y0 + thick,          length - thick, thick, val);
    err |= drawVerticalLine  (x0 + width - thick, y0 + thick,          length - thick, thick, val);
    return err;
}

bool canvas::drawBoxFill(int32_t x0, int32_t y0, int32_t length, int32_t width, bool val)
{
    if (ptr != nullptr) {
        int32_t x = 0, y = 0;
        for (y = 0; y < length; y++)
            for (x = 0; x < width; x++)
                setPixle(x0 + x, y0 + y, val);
        return 0;
    }
    return 1;
}

/*
 * Call-back routine for merged image output
 */
static int line_out(const struct jbg85_dec_state* s, unsigned char* start, size_t len, unsigned long y, void* file)
{
    /* this is awfull and needs work, infact most of this is awfull lol*/
    if (file != nullptr) {
        jbg_buffer* buf = (jbg_buffer*)file;

        if (y == 0) {
            if (buf->__img != nullptr) {
                free(buf->__img);
                buf->__img = nullptr;
                buf->__img_len = 0;
            }

            if (buf->__img == nullptr) {
                buf->__img = (uint8_t*) malloc(len);
                if (buf->__img != nullptr) {
                    memcpy(buf->__img, start, len);
                    buf->__img_len += len;
                }
            }
            return 0;
        }

        if (buf->__img != nullptr) {
            uint8_t* org = buf->__img;
            buf->__img = (uint8_t*)realloc(org, buf->__img_len + len);
            if (buf->__img != nullptr) {
                memcpy(((uint8_t*)buf->__img) + buf->__img_len, start, len);
                buf->__img_len += len;
            } else {
                free(org);
                org = nullptr;
            }
        }
    }

    return 0;
}

uint8_t* canvas::jbg_open(const char* fileName, uint32_t* x0, uint32_t* y0, uint32_t* _size)
{
    jbg_buffer buf{ nullptr };
    struct jbg85_dec_state s;

    int result;
    unsigned char* inbuf, * outbuf;
    size_t bytes_read = 0, len, cnt;
    size_t inbuflen = 1024, outbuflen;
    unsigned long xmax = COMP_XMAX;

    inbuf = (unsigned char*)malloc(inbuflen);
    outbuflen = ((xmax >> 3) + !!(xmax & 7)) * 3;
    outbuf = (unsigned char*)malloc(outbuflen);

    if (!inbuf || !outbuf) {
        return nullptr;
    }

    FILE* fd;
    fd = fopen(fileName, "rb");

    if (fd != NULL) 
    {
        /* send input file to decoder */
        jbg85_dec_init(&s, outbuf, outbuflen, line_out, (void*)&buf);
        result = JBG_EAGAIN;
        while ((len = fread(inbuf, 1, inbuflen, fd)))
        {
            result = jbg85_dec_in(&s, inbuf, len, &cnt);
            bytes_read += cnt;
            //printf("%s\n", jbg85_strerror(result));
            if (result != JBG_EAGAIN)
                break;
        }

        if (result != JBG_EOK) {
            if (buf.__img != nullptr) {
                free(buf.__img);
                buf.__img = nullptr;
                buf.__img_len = 0;
            }
        }
        
        free(inbuf);
        free(outbuf);
        fclose(fd);
    }

    /* got the image */
    if (buf.__img != nullptr) {
        *y0 = jbg85_dec_getheight(&s);
        *x0 = jbg85_dec_getwidth(&s);
        *_size = buf.__img_len;
        return buf.__img;
    }

    return nullptr;
}

//TODO: add file io error handling
uint8_t* canvas::img_open(const char* fileName, uint32_t *x0, uint32_t *y0, uint32_t *_size)
{
    uint32_t image_size = 0;
    uint8_t* image = nullptr;

    header bmpHead = { 0x00 };
    infoHeader bmpInfoHead = { 0x00 };

    FILE* fd;
    fd = fopen(fileName, "rb");

    if (fd != NULL) {
        fseek(fd, 0, SEEK_END);
        size_t file_size = ftell(fd);
        fseek(fd, 0, SEEK_SET);

        if (file_size >= 0x0036) {
            //no error checking, yet!
            //fread will fail hard!
            /////////////////////////////////////////////////////////////////////////
            fread(&bmpHead.Signature, sizeof(uint16_t), 1, fd);
            fread(&bmpHead.FileSize, sizeof(uint32_t), 1, fd);
            fread(&bmpHead.reserved, sizeof(uint32_t), 1, fd);
            fread(&bmpHead.DataOffset, sizeof(uint32_t), 1, fd);
            /////////////////////////////////////////////////////////////////////////
            fread(&bmpInfoHead.Size, sizeof(uint32_t), 1, fd);
            fread(&bmpInfoHead.Width, sizeof(uint32_t), 1, fd);
            fread(&bmpInfoHead.Height, sizeof(uint32_t), 1, fd);
            fread(&bmpInfoHead.Planes, sizeof(uint16_t), 1, fd);
            fread(&bmpInfoHead.Bits_Per_Pixel, sizeof(uint16_t), 1, fd);
            fread(&bmpInfoHead.Compression, sizeof(uint32_t), 1, fd);
            fread(&bmpInfoHead.ImageSize, sizeof(uint32_t), 1, fd);
            fread(&bmpInfoHead.XpixelsPerM, sizeof(uint32_t), 1, fd);
            fread(&bmpInfoHead.YpixelsPerM, sizeof(uint32_t), 1, fd);
            fread(&bmpInfoHead.Colors_Used, sizeof(uint32_t), 1, fd);
            fread(&bmpInfoHead.Important_Colors, sizeof(uint32_t), 1, fd);
            /* bmp check is not very good */
            if ((bmpInfoHead.Bits_Per_Pixel != 24) || (bmpHead.Signature != 0x4D42) ||
                (bmpInfoHead.Compression != 0) || (bmpInfoHead.Colors_Used != 0) ||
                (bmpHead.FileSize - 0x36 != bmpInfoHead.ImageSize)) {
                fclose(fd);
                return nullptr;
            }

            /* reads image */
            image_size = bmpInfoHead.ImageSize;
            image = new uint8_t[image_size];
            fread(image, sizeof(uint8_t), image_size, fd);
        }
        fclose(fd);
    }

    /* got the image */
    if (image != nullptr) {
        *y0 = bmpInfoHead.Height;
        *x0 = bmpInfoHead.Width;
        *_size = bmpInfoHead.ImageSize;
        return image;
    }

    return nullptr;
}

uint8_t* canvas::pbm_open(const char* fileName, uint32_t* x0, uint32_t* y0, uint32_t* _size)
{
    uint32_t dim[2] = { 0x00 };
    uint32_t image_size = 0;
    uint8_t* image = nullptr;

    FILE* fd;
    fd = fopen(fileName, "rb");

    if (fd != NULL) {
        fseek(fd, 0, SEEK_END);
        size_t file_size = ftell(fd);
        fseek(fd, 0, SEEK_SET);
       
        int i = 0;
        char* pch;
        bool err = 0;
        char trash[500];
        

        if (fgets(trash, 5, fd) != NULL) {
            if (strcmp(trash, "P4\n"))
            {
                fclose(fd);
                return nullptr;
            }
        } else {
            fclose(fd);
            return nullptr;
        }
 
        do {
            if (fgets(trash, 500, fd) == NULL) {
                fclose(fd);
                return nullptr;
            }
        }
        while (trash[0] == '#');

        pch = strtok(trash, " \n");
        while (pch != NULL) {
            dim[i++] = atoi(pch);
            pch = strtok(NULL, " \n");
        }

        image_size = (uint32_t)(file_size - ftell(fd));
        image = new uint8_t[image_size];

        if (image != nullptr) {
            fread(image, sizeof(uint8_t), image_size, fd);
        }

        fclose(fd);
    }

    /* got the image */
    if (image != nullptr) {
        *y0 = dim[1];
        *x0 = dim[0];
        * _size = image_size;
        return image;
    }

    return nullptr;
}

bool canvas::import_jbg(const char* fileName)
{
    uint32_t x = 0, y = 0, s = 0;
    uint8_t* image_jbg = jbg_open(fileName, &x, &y, &s);

    if (image_jbg != nullptr) {

        uint32_t x1 = s / y;
        uint32_t q, _y, _x = 0;
        uint8_t* tmp;

        if (create(x, y, 0) != 1) {
            for (_y = 0; _y < y; _y++) {
                q = x1 * _y;
                tmp = image_jbg + q;

                uint8_t bit = 0;
                for (_x = 0; _x < x; _x++) {
                    if (BIT_CHECK(tmp[_x / 8], 7-bit))
                        setPixle(_x, _y, 1);
                    bit = (bit + 1) % 8;
                }
            }
        } else {
            free(image_jbg);
            return 1;
        }

        free(image_jbg);
    }

    return 0;
}


//TODO: add file io error handling
bool canvas::import_24bit(const char* fileName, DITHER type)
{
    double lum;
    uint32_t y0 = 0, x0 = 0, x1 = 0, q = 0;
    uint32_t x = 0, y = 0, s = 0;
    float* gray = nullptr;

    uint8_t* row = nullptr;
    uint8_t* image_24 = img_open(fileName, &x, &y, &s);

    if (image_24 != nullptr) {
        uint32_t i_x = s / y, i_y = y, gray_size = x * i_y;

        gray = new float[gray_size];
        float* tmp = gray;
    
        if (gray != nullptr) {
            for (y0 = 0; y0 < i_y; y0++) {
                x1 = 0;
                row = &image_24[i_x * y0];

                for (x0 = 0; x0 < x; x0++) {
                    //https://stackoverflow.com/questions/4147639/converting-color-bmp-to-grayscale-bmp
                    lum = /*blue*/(row[x1++] * 0.11) + /*green*/(row[x1++] * 0.59) + /*red*/(row[x1++] * 0.30);
                    if (lum < 0) lum = 0;
                    if (lum > 255) lum = 255;
                    *tmp = (uint8_t)lum;
                    tmp++;
                }
            }
            delete[] image_24;
        
            img _img = { gray, x, i_y }; //create image struct

            bool err = 0;
            if ((err |= dither(&_img, type)) != 1) {
                x1 = i_y - 1; //reused, not x, but y
                if ((err |= create(x, i_y, 0)) != 1) {
                    for (y0 = 0; y0 < i_y; y0++) {
                        for (x0 = 0; x0 < x; x0++) {
                            q = ((((x1)-y0) * x)) + x0;
                            tmp = gray + q;
                            s = (uint32_t) *tmp;

                            if (s == 0)
                                setPixle(x0, y0, 1);
                        }
                    }
                }
            }
            delete[] gray;
            return err;
        }
        delete[] image_24;
        return 1;
    }

    return 1;
}

bool canvas::import_pbm(const char* fileName)
{
    uint32_t x = 0, y = 0, s = 0;
    uint8_t* image_pbm = pbm_open(fileName, &x, &y, &s);

    if (image_pbm != nullptr) {

        uint32_t x1 = s / y;
        uint32_t q, _y, _x = 0;
        uint8_t* tmp;

        if (create(x, y, 0) != 1) {
            for (_y = 0; _y < y; _y++) {
                q = x1 * _y;
                tmp = image_pbm + q;

                uint8_t bit = 0;
                for (_x = 0; _x < x; _x++) {
                    if (BIT_CHECK(tmp[_x / 8], 7 - bit))
                        setPixle(_x, _y, 1);
                    bit = (bit + 1) % 8;
                }
            }
        }
        else {
            delete[] image_pbm;
            return 1;
        }

        delete[] image_pbm;
    }

    return 0;
}

bool canvas::mirror(MIRROR m)
{
    if (ptr != nullptr)
    {
        bool* g;
        bool* tmp;
        if (m == MIRROR::Horizontal) {
            tmp = new bool[_x * sizeof(bool)];
            if (tmp != nullptr) {
                for (uint32_t y = 0; y < _y; y++) {
                    g = &ptr[y * _x];
                    for (uint32_t x = 0; x < _x; x++) {
                        tmp[(_x - 1) - x] = g[x];
                    }
                    memcpy(g, tmp, _x * sizeof(bool));
                }
                delete[] tmp;
                return 0;
            }
            return 1;
        }
        else if (m == MIRROR::Vertical) //TODO: fix, make "more better"
        {
            uint32_t q = _x * _y;
            tmp = new bool[q * sizeof(bool)];
            if (tmp != nullptr) {
                memcpy(tmp, ptr, q * sizeof(bool));
                for (uint32_t y = 0; y < _y; y++) {
                    g = &ptr[(_y - 1 - y) * _x];
                    memcpy(g, &tmp[y * _x], _x * sizeof(bool));
                }
                delete[] tmp;
                return 0;
            }
            return 1;
        }
        return 1;
    }
    return 1;
}

bool canvas::rotate(DEGREE rot)
{
    if (ptr != nullptr) {
        uint32_t y = 0, x = 0;
        uint32_t y1 = 0, x1 = 0;

        uint32_t _y_new = _y, _x_new = _x;
        uint32_t q = _x_new * _y_new;
        size_t _asize = q * sizeof(bool);

        if (rot == DEGREE::ROT_0 || rot == DEGREE::ROT_360)
            return 0;

        if (rot == DEGREE::ROT_90 || rot == DEGREE::ROT_270) {
            _y_new = _x;
            _x_new = _y;
        }

        bool* tmp;
        bool* new_ptr = new bool[_asize] {0};

        if (new_ptr != nullptr) {
            for (y = 0; y < _y; y++) {
                for (x = 0; x < _x; x++) {
                    if (rot == DEGREE::ROT_90)  { x1 = (_y - 1) - y; y1 = x; }
                    if (rot == DEGREE::ROT_180) { x1 = (_x - 1) - x; y1 = (_y - 1) - y; }
                    if (rot == DEGREE::ROT_270) { x1 = y;            y1 = (_x - 1) - x; }

                    q = (y1 * _x_new);
                    tmp = new_ptr + q + x1;
                    *tmp = ptr[(y * _x) + x];
                }
            }

            delete[] ptr;
            ptr = new_ptr;
            _y = _y_new;
            _x = _x_new;
            return 0;
        }
        return 1;
    }
    return 1;
}

bool canvas::dither(img* image, DITHER type)
{
    bool err = 0;
    switch (type)
    {
    case DITHER::FloydSteinberg:
        err |= floydSteinberg(image);
    case DITHER::Stucki:
        err |=  stucki(image);
    case DITHER::Jarvis:
        err |= jarvis(image);
    case DITHER::Atkinson:
        err |= atkinson(image);
    case DITHER::Bayer_2x2:
        err |= bayer(image, 0);
    case DITHER::Bayer_4x4:
        err |= bayer(image, 1);
    case DITHER::Bayer_8x8:
        err |= bayer(image, 2);
    case DITHER::Bayer_16x16:
        err |= bayer(image, 3);
    case DITHER::Cluster:
        err |= cluster(image);
    default:
        err |= threshold(image, 256 / 2);
    };

    return err;
}

float canvas::_img_get(img* image, uint32_t x0, uint32_t y0)
{
    if (x0 < image->__img_x && y0 < image->__img_y)
        return image->__img[(image->__img_x * y0) + x0];

    return 0;
}

bool canvas::_img_set(img* image, uint32_t x0, uint32_t y0, float value)
{
    if (x0 < image->__img_x && y0 < image->__img_y)
        image->__img[(image->__img_x * y0) + x0] = value;

    return 0;
}

bool canvas::threshold(img* image, uint8_t value)
{
    if (image->__img != nullptr) {
        uint8_t newpixel;
        uint32_t y = 0, x = 0;
        for (y = 0; y < image->__img_y; y++) {
            for (x = 0; x < image->__img_x; x++) {
                newpixel = (_img_get(image, x, y) < value) ? 0 : 255;
                _img_set(image, x, y, newpixel);
            }
        }
        return 0;
    }
    return 1;
}

/* https://imagej.net/Dithering */
bool canvas::floydSteinberg(img* image)
{   
    if (image->__img != nullptr) {
        uint32_t y = 0, x = 0;
        float oldpixel, newpixel;
        float quant_error;
        float w1 = 7.0 / 16.0, w2 = 3.0 / 16.0, w3 = 5.0 / 16.0, w4 = 1.0 / 16.0;

        for (y = 0; y < image->__img_y; y++) {
            for (x = 0; x < image->__img_x; x++) {
                oldpixel = _img_get(image, x, y);
                newpixel = (oldpixel < 128) ? 0.0f : 255.0f;
                _img_set(image, x, y, newpixel);
                quant_error = (oldpixel - newpixel);

                _img_set(image, x + 1, y,      (_img_get(image, x + 1, y)      + w1 * quant_error));
                _img_set(image, x - 1, y + 1,  (_img_get(image, x - 1, y + 1)  + w2 * quant_error));
                _img_set(image, x,     y + 1,  (_img_get(image, x,     y + 1)  + w3 * quant_error));
                _img_set(image, x + 1, y + 1,  (_img_get(image, x + 1, y + 1)  + w4 * quant_error));
            }
        }
        return 0;
    }
    return 1;
}

/* https://imagej.net/Dithering */
bool canvas::stucki(img* image)
{
    if (image->__img != nullptr) {
        uint32_t y = 0, x = 0;
        float quant_error;
        float oldpixel, newpixel; //uint8_t ?
        float w8 = (float)(8.0 / 42.0),
              w7 = (float)(7.0 / 42.0),
              w5 = (float)(5.0 / 42.0),
              w4 = (float)(4.0 / 42.0),
              w2 = (float)(2.0 / 42.0),
              w1 = (float)(1.0 / 42.0);

        for (y = 0; y < image->__img_y; y++) {
            for (x = 0; x < image->__img_x; x++) {
                oldpixel = _img_get(image, x, y);
                newpixel = (oldpixel < 128) ? 0.0f : 255.0f;
                _img_set(image, x, y, newpixel);
                quant_error = (oldpixel - newpixel);
                _img_set(image, x + 1, y,     (_img_get(image, x + 1, y)     + w7 * quant_error));
                _img_set(image, x + 2, y,     (_img_get(image, x + 2, y)     + w5 * quant_error));
                _img_set(image, x - 2, y + 1, (_img_get(image, x - 2, y + 1) + w2 * quant_error));
                _img_set(image, x - 1, y + 1, (_img_get(image, x - 1, y + 1) + w4 * quant_error));
                _img_set(image, x,     y + 1, (_img_get(image, x,     y + 1) + w8 * quant_error));
                _img_set(image, x + 1, y + 1, (_img_get(image, x + 1, y + 1) + w4 * quant_error));
                _img_set(image, x + 2, y + 1, (_img_get(image, x + 2, y + 1) + w2 * quant_error));
                _img_set(image, x - 2, y + 2, (_img_get(image, x - 2, y + 2) + w1 * quant_error));
                _img_set(image, x - 1, y + 2, (_img_get(image, x - 1, y + 2) + w2 * quant_error));
                _img_set(image, x,     y + 2, (_img_get(image, x,     y + 2) + w4 * quant_error));
                _img_set(image, x + 1, y + 2, (_img_get(image, x + 1, y + 2) + w2 * quant_error));
                _img_set(image, x + 2, y + 2, (_img_get(image, x + 2, y + 2) + w1 * quant_error));
            }
        }
        return 0;
    }
    return 1;
}

/* https://imagej.net/Dithering */
bool canvas::jarvis(img* image)
{
    if (image->__img != nullptr) {
        uint32_t y = 0, x = 0;
        float quant_error;
        float oldpixel, newpixel; //uint8_t ?
        float w7 = (float)(7.0 / 48.0),
              w5 = (float)(5.0 / 48.0),
              w3 = (float)(3.0 / 48.0),
              w1 = (float)(1.0 / 48.0);

        for (y = 0; y < image->__img_y; y++) {
            for (x = 0; x < image->__img_x; x++) {
                oldpixel = _img_get(image, x, y);
                newpixel = (oldpixel < 128) ? 0.0f : 255.0f;
                _img_set(image, x, y, newpixel);
                quant_error = (oldpixel - newpixel);
                _img_set(image, x + 1, y,     (_img_get(image, x + 1, y)     + w7 * quant_error));
                _img_set(image, x + 2, y,     (_img_get(image, x + 2, y)     + w5 * quant_error));
                _img_set(image, x - 2, y + 1, (_img_get(image, x - 2, y + 1) + w3 * quant_error));
                _img_set(image, x - 1, y + 1, (_img_get(image, x - 1, y + 1) + w5 * quant_error));
                _img_set(image, x,     y + 1, (_img_get(image, x,     y + 1) + w7 * quant_error));
                _img_set(image, x + 1, y + 1, (_img_get(image, x + 1, y + 1) + w5 * quant_error));
                _img_set(image, x + 2, y + 1, (_img_get(image, x + 2, y + 1) + w3 * quant_error));
                _img_set(image, x - 2, y + 2, (_img_get(image, x - 2, y + 2) + w1 * quant_error));
                _img_set(image, x - 1, y + 2, (_img_get(image, x - 1, y + 2) + w3 * quant_error));
                _img_set(image, x,     y + 2, (_img_get(image, x,     y + 2) + w5 * quant_error));
                _img_set(image, x + 1, y + 2, (_img_get(image, x + 1, y + 2) + w3 * quant_error));
                _img_set(image, x + 2, y + 2, (_img_get(image, x + 2, y + 2) + w1 * quant_error));
            }
        }
        return 0;
    }
    return 1;
}

/* https://imagej.net/Dithering */
bool canvas::atkinson(img* image)
{
    if (image->__img != nullptr) {
        uint32_t y = 0, x = 0;
        float quant_error;
        float oldpixel, newpixel; //uint8_t ?
        float w1 = (float)(1.0 / 8.0);

        for (y = 0; y < image->__img_y; y++) {
            for (x = 0; x < image->__img_x; x++) {
                oldpixel = _img_get(image, x, y);
                newpixel = (oldpixel < 128) ? 0.0f : 255.0f;
                _img_set(image, x, y, newpixel);
                quant_error = (oldpixel - newpixel);
                _img_set(image, x + 1, y,     (_img_get(image, x + 1, y)     + w1 * quant_error));
                _img_set(image, x + 2, y,     (_img_get(image, x + 2, y)     + w1 * quant_error));
                _img_set(image, x - 1, y + 1, (_img_get(image, x - 1, y + 1) + w1 * quant_error));
                _img_set(image, x,     y + 1, (_img_get(image, x,     y + 1) + w1 * quant_error));
                _img_set(image, x + 1, y + 1, (_img_get(image, x + 1, y + 1) + w1 * quant_error));
                _img_set(image, x,     y + 2, (_img_get(image, x,     y + 2) + w1 * quant_error));
            }
        }
        return 0;
    }
    return 1;
}

/* https://imagej.net/Dithering */
bool canvas::bayer(img* image, uint8_t matrix)
{
    if (image->__img != nullptr) {
        uint32_t y = 0, x = 0;
        uint8_t newpixel;
        uint8_t* d = (uint8_t*)matrix_array[matrix];
        matrix = 2 << matrix;

        for (y = 0; y < image->__img_y; y++) {
            for (x = 0; x < image->__img_x; x++) {
                newpixel = (_img_get(image, x, y) >= d[(y % matrix * matrix + x % matrix)]) ? 255 : 0;
                _img_set(image, x, y, newpixel);
            }
        }
        return 0;
    }
    return 1;
}

/* https://imagej.net/Dithering */
bool canvas::cluster(img* image)
{
    if (image->__img != nullptr) {
        uint32_t y = 0, x = 0;
        uint8_t newpixel;

        for (y = 0; y < image->__img_y; y++) {
            for (x = 0; x < image->__img_x; x++) {
                newpixel = (_img_get(image, x, y) >= c_4x4[(y % 4 + x % 4)]) ? 255 : 0;
                _img_set(image, x, y, (uint8_t)newpixel);
            }
        }
        return 0;
    }
    return 1;
}