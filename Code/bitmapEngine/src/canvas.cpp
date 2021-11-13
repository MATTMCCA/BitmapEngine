/*
    ***************************************************************************************
    MJM -2021-
    canvas / bitmap Lib
    
    ======================================================================================= 
    
    File: canvas.cpp
    
        Notes: 
        
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
    bool err = 0;

    uint32_t BMPDATASIZE = 0;
    uint8_t* BMPDATA = nullptr;
    uint32_t row = 0;

    if (ptr != nullptr) {

        if ((err |= _pack_bool(&BMPDATA, &row)) == 0)
            BMPDATASIZE = row * _y;

        if ((BMPDATA != nullptr) && (err == 0)) {
            uint32_t y;
            struct jbg85_enc_state s;            

            FILE* fd;
            fopen_s(&fd, fileName, "wb"); //fopen(fileName, "wb");

            if (fd != NULL) {
                jbg85_enc_init(&s, _x, _y, data_out, fd);
                jbg85_enc_options(&s, JBG_TPBON, 0, -1);

                unsigned char* next_line = nullptr, * prev_line = nullptr, * prevprev_line = nullptr;

                for (y = 0; y < _y; y++) {
                    /* Use a 3-line ring buffer, because the encoder requires that the two
                     * previously supplied lines are still in memory when the next line is
                     * processed. */
                    next_line = &BMPDATA[y * row];

                    /* JBIG compress another line and write out result via callback */
                    jbg85_enc_lineout(&s, next_line, prev_line, prevprev_line);
                    prevprev_line = prev_line;
                    prev_line = next_line;

                    /* adjust final image height via NEWLEN */
                    if (y == _y)
                        jbg85_enc_newlen(&s, _y);
                }
                if ((fclose(fd) != 0) || (ferror(fd)))
                    err |= 1;
            }
            else err |= 1;
            delete[] BMPDATA; //release when done
        }
        else err |= 1;
    }
    else err |= 1;

    return err;
}

bool canvas::savePBM(const char* fileName)
{
    bool err = 0;

    uint32_t BMPDATASIZE = 0;
    uint8_t* BMPDATA = nullptr;
    uint32_t row = 0;

    if (ptr != nullptr) {

        if ((err |= _pack_bool(&BMPDATA, &row)) == 0)
            BMPDATASIZE = row * _y;

        if ((BMPDATA != nullptr) && (err == 0)) {

            FILE* fd;
            fopen_s(&fd, fileName, "wb"); //fopen(fileName, "wb");

            if (fd != NULL) {
                fprintf(fd, "P4\n# Created by BitmapEngine\n%d %d\n", _x, _y);
                fwrite(BMPDATA, sizeof(uint8_t), BMPDATASIZE, fd);
                if ((fclose(fd) != 0) || (ferror(fd)))
                    err |= 1;
            }
            else err |= 1;
            delete[] BMPDATA; //release when done
        }
        else err |= 1;
    }
    else err |= 1;

    return err;
}

/* mainly for debugging */
bool canvas::saveXBM(const char* fileName, const char* structName)
{
    bool err = 0;

    uint32_t BMPDATASIZE = 0;
    uint8_t* BMPDATA = nullptr;
    uint32_t row = 0, k = 1;

    if (ptr != nullptr) {

        if ((err |= _pack_bool(&BMPDATA, &row)) == 0)
            BMPDATASIZE = row * _y;

        if ((BMPDATA != nullptr) && (err == 0)) {

            FILE* fd;            
            fopen_s(&fd, fileName, "wb"); //fopen(fileName, "wb");

            if (fd != NULL) {
                fprintf(fd, "#define IMG_width %d\n", _x);
                fprintf(fd, "#define IMG_height %d\n", _y);
                fprintf(fd, "static unsigned char IMG_bits[] = {\n\t");

                /* jank lookup table */
                uint8_t xbm_table[256];
                for (int i = 0; i < 256; i++) {
                    int k = i;
                    xbm_table[i] = 0;
                    for (int b = 0; b < 8; b++) {
                        xbm_table[i] <<= 1;
                        xbm_table[i] |= k & 1;
                        k >>= 1;
                    }
                    xbm_table[i] &= 0xFF;
                }

                for (uint32_t i = 0, k = 1; i < BMPDATASIZE; i++, k++) {
                    fprintf(fd, "0x%.2x%s", xbm_table[BMPDATA[i]], k != BMPDATASIZE ? ", " : "\n};");
                    if ((k % row) == 0)
                        fprintf(fd, "\n\t");
                }
                              
                if ((fclose(fd) != 0) || ferror(fd))
                    err |= 1;
            }
            else err |= 1;
            delete[] BMPDATA; //release when done
        }
        else err |= 1;
    }
    else err |= 1;

    return err;
}

bool canvas::saveBMP(const char* fileName, int DPI)
{
    bool err = 0;

    uint32_t BMPDATASIZE = 0;
    uint8_t* BMPDATA = nullptr;
    uint32_t row = 0;

    if (ptr != nullptr) {

        if((err |= _pack_bool(&BMPDATA, &row, 0, 4)) == 0)
            BMPDATASIZE = row * _y;

        if ((BMPDATA != nullptr) && (err == 0)) 
        {
            header bmpHead;
            bmpHead.Signature = 0x4D42;
            bmpHead.FileSize = 54 /*header*/ + 8 /*color table*/ + (BMPDATASIZE) /*bytes*/;
            bmpHead.reserved = 0x00;
            bmpHead.DataOffset = 62;

            infoHeader bmpInfoHead;
            bmpInfoHead.Size = 40;
            bmpInfoHead.Width = _x;
            bmpInfoHead.Height = _y;
            bmpInfoHead.Planes = 0x01;
            bmpInfoHead.Bits_Per_Pixel = 0x01;
            bmpInfoHead.Compression = 0x00;
            bmpInfoHead.ImageSize = BMPDATASIZE;
            bmpInfoHead.XpixelsPerM = (uint32_t)(DPI * 39.3701);
            bmpInfoHead.YpixelsPerM = (uint32_t)(DPI * 39.3701);
            bmpInfoHead.Colors_Used = 0x00;
            bmpInfoHead.Important_Colors = 0x00;

            FILE* fd;            
            fopen_s(&fd, fileName, "wb"); //fopen(fileName, "wb");

            if (fd != NULL) {
                fwrite(&bmpHead.Signature, sizeof(uint16_t), 1, fd);
                fwrite(&bmpHead.FileSize, sizeof(uint32_t), 1, fd);
                fwrite(&bmpHead.reserved, sizeof(uint32_t), 1, fd);
                fwrite(&bmpHead.DataOffset, sizeof(uint32_t), 1, fd);
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
                fwrite(&COLOR_TABLE, sizeof(uint32_t), 2, fd);
                fwrite(BMPDATA, sizeof(uint8_t), BMPDATASIZE, fd);

                if ((fclose(fd) != 0) || ferror(fd))
                    err |= 1;
            }
            else err |= 1;
            delete[] BMPDATA; //release when done
        }
        else err |= 1;
    }
    else err |= 1;

    return err;
}

/* MICROCOM COMPRESSION BINARY, Mode 2, no flash header */
// https://microcomcorp.com/downloads/bmp2mic.zip
bool canvas::saveMCB(const char* fileName, int headSize)
{
    bool err = 0;

    uint32_t BMPDATASIZE = 0;
    uint8_t* BMPDATA = nullptr;
    uint32_t row = 0, i = 0;
    uint8_t temp;

    /* fit to head, messy..... */
    canvas* tmp = new canvas;
    err |= tmp->create(headSize, _y, 0);
    err |= tmp->addSprite(this, ((headSize - _x) / 2), 0, 0);
    delete[] ptr; ptr = nullptr;
    err |= create(tmp->ptr, tmp->get_x(), tmp->get_y(), 0);
    delete tmp;

    // needed for format
    err |= rotate(DEGREE::ROT_180); //idk why its rot-180

    if ((err |= _pack_bool(&BMPDATA, &row)) == 0)
        BMPDATASIZE = row * _y;

    if ((BMPDATA != nullptr) && (err == 0)) {

        FILE* fd;
        fopen_s(&fd, fileName, "wb");
        if (fd != NULL) {

            while (i < BMPDATASIZE) {
                temp = BMPDATA[i++];
                if (i > BMPDATASIZE) break;
                if ((temp == 0x00) || (temp == 0xFF)) {
                    int cnt = 0;
                    fwrite(&temp, sizeof(uint8_t), 1, fd);                    
                    while ((i < BMPDATASIZE) && (BMPDATA[i] == temp) && (cnt < 0xFE)) {
                        i++;
                        cnt++;
                    }
                    temp = (uint8_t)cnt;
                    fwrite(&temp, sizeof(uint8_t), 1, fd);
                }
                else {
                    fwrite(&temp, sizeof(uint8_t), 1, fd);
                }
            }

            if ((fclose(fd) != 0) || (ferror(fd)))
                err |= 1;
        }
        else err |= 1;
        delete[] BMPDATA; //release when done
    }

    return err;
}

bool canvas::_pack_bool(uint8_t** BMPDATA, uint32_t* row, bool ort, int byte_bound)
{
    bool err = 0; 
    if (*BMPDATA != nullptr) delete[] *BMPDATA;
    *BMPDATA = nullptr;

    if (ptr != nullptr) {
        uint32_t image_y = _y;
        uint32_t image_x = (uint32_t)(_x / 8.0);

        if ((image_x * 8) < (uint32_t)_x)   image_x++;   //math fix
        if (image_x == 0)                   image_x = 1; //non 0 byte width
        while ((image_x % byte_bound))      image_x++;   //byte binding
        
        uint32_t BMPDATASIZE = (image_x * image_y) * sizeof(uint8_t);

        *row = image_x;
        *BMPDATA = new uint8_t[BMPDATASIZE]{ 0x00 };       

        if (*BMPDATA != nullptr) {
            if (ort == 0) {
                image_y--;
                memset(*BMPDATA, 0xFF, BMPDATASIZE);
            }
            else memset(*BMPDATA, 0x00, BMPDATASIZE);

            bool* in_line;
            uint8_t* out_line;
            uint32_t __y = 0, __x = 0;

            for (__y = 0; __y < _y; __y++) {
                in_line = &ptr[(__y * _x)];
                out_line = (ort == 0) ? (*BMPDATA + ((image_y - __y) * image_x)) : (*BMPDATA + (__y * image_x));

                for (__x = 0; __x < _x; __x++) {
                    if (in_line[__x] ^ _inv) {
                        if (ort == 0) BIT_CLEAR(out_line[(__x / 8)], (7 - (__x % 8)));
                        else         BIT_SET(out_line[(__x / 8)], (7 - (__x % 8)));
                    }
                }
            }
        }
        else err |= 1;
    }
    else err |= 1;

    return err;
}

bool canvas::invert(bool invert)
{
    _inv = invert;
    return 0;
}

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
    ptr = nullptr;
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
                    buf->__img_len += (uint32_t) len;
                }
            }
            return 0;
        }

        if (buf->__img != nullptr) {
            uint8_t* org = buf->__img;
            buf->__img = (uint8_t*)realloc(org, buf->__img_len + len);
            if (buf->__img != nullptr) {
                memcpy(((uint8_t*)buf->__img) + buf->__img_len, start, len);
                buf->__img_len += (uint32_t) len;
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
    *x0 = *y0 = *_size = 0;
    int result;
    unsigned char* inbuf, * outbuf;
    size_t bytes_read = 0, len, cnt;
    size_t inbuflen = 1024, outbuflen;
    unsigned long xmax = COMP_XMAX; // working buffer, make larger on fail

    jbg_buffer buf{ nullptr };
    struct jbg85_dec_state s;

    inbuf = (unsigned char*)malloc(inbuflen);
    outbuflen = ((xmax >> 3) + !!(xmax & 7)) * 3; //<--- this line, not my code, im not fixing it since it works.
    outbuf = (unsigned char*)malloc(outbuflen);

    if (!inbuf || !outbuf) {
        return nullptr;
    }

    FILE* fd;
    //fopen(fileName, "rb");
    fopen_s(&fd, fileName, "rb");

    if (fd != NULL) {
        /* send input file to decoder */
        jbg85_dec_init(&s, outbuf, outbuflen, line_out, (void*)&buf);
        result = JBG_EAGAIN;
        while ((len = fread(inbuf, 1, inbuflen, fd))) {
            result = jbg85_dec_in(&s, inbuf, len, &cnt);
            bytes_read += cnt;
            if ((result != JBG_EAGAIN) || (ferror(fd)))
                break;
        }

        if ((result != JBG_EOK) || (ferror(fd))) {
            if (buf.__img != nullptr) 
                free(buf.__img);
            buf.__img = nullptr;
            buf.__img_len = 0;
        } else {
            *y0 = jbg85_dec_getheight(&s);
            *x0 = jbg85_dec_getwidth(&s);
            *_size = buf.__img_len;
        }
        
        free(inbuf);
        free(outbuf);
        fclose(fd);
    }

    return buf.__img;
}

uint8_t* canvas::bmp_open(const char* fileName, uint32_t* x0, uint32_t* y0, uint32_t* _size)
{
    *x0 = *y0 = *_size = 0;
    uint32_t image_size = 0;
    uint8_t* image = nullptr;

    header bmpHead = { 0x00 };
    infoHeader bmpInfoHead = { 0x00 };

    FILE* fd;
    //fopen(fileName, "rb");
    fopen_s(&fd, fileName, "rb");

    if (fd != NULL) {
        fseek(fd, 0, SEEK_END);
        size_t file_size = ftell(fd);
        fseek(fd, 0, SEEK_SET);

        fread(&bmpHead.Signature,               sizeof(uint16_t), 1, fd);
        fread(&bmpHead.FileSize,                sizeof(uint32_t), 1, fd);
        fread(&bmpHead.reserved,                sizeof(uint32_t), 1, fd);
        fread(&bmpHead.DataOffset,              sizeof(uint32_t), 1, fd);
        fread(&bmpInfoHead.Size,                sizeof(uint32_t), 1, fd);
        fread(&bmpInfoHead.Width,               sizeof(uint32_t), 1, fd);
        fread(&bmpInfoHead.Height,              sizeof(uint32_t), 1, fd);
        fread(&bmpInfoHead.Planes,              sizeof(uint16_t), 1, fd);
        fread(&bmpInfoHead.Bits_Per_Pixel,      sizeof(uint16_t), 1, fd);
        fread(&bmpInfoHead.Compression,         sizeof(uint32_t), 1, fd);
        fread(&bmpInfoHead.ImageSize,           sizeof(uint32_t), 1, fd);
        fread(&bmpInfoHead.XpixelsPerM,         sizeof(uint32_t), 1, fd);
        fread(&bmpInfoHead.YpixelsPerM,         sizeof(uint32_t), 1, fd);
        fread(&bmpInfoHead.Colors_Used,         sizeof(uint32_t), 1, fd);
        fread(&bmpInfoHead.Important_Colors,    sizeof(uint32_t), 1, fd);

        if (ferror(fd) == 0) {
            if ((bmpInfoHead.Bits_Per_Pixel == 24) && (bmpHead.Signature == 0x4D42) &&
                (bmpInfoHead.Compression == 0) && (bmpInfoHead.Colors_Used == 0)) {
                image_size = bmpInfoHead.ImageSize;
                image = new uint8_t[image_size];
                if (image != nullptr) {
                    fread(image, sizeof(uint8_t), image_size, fd);
                    if (ferror(fd)) {
                        delete[] image;
                        image = nullptr;
                    } else {
                        *y0 = bmpInfoHead.Height;
                        *x0 = bmpInfoHead.Width;
                        *_size = bmpInfoHead.ImageSize;
                    }
                }
            }
        }
        fclose(fd);
    }

    /* convert to grayscale */
    if (image != nullptr) {
        double lum = 0;
        uint8_t* row = nullptr, * tmp = nullptr;
        uint32_t __y = 0, __x = 0, x1 = 0, _fb = 0, _x = *_size / *y0;

        for (__y = 0; __y < *y0; __y++) {
            x1 = 0;
            row = &image[_x * __y];
            for (__x = 0; __x < *x0; __x++) {
                tmp = &image[_fb++];
                //https://stackoverflow.com/questions/4147639/converting-color-bmp-to-grayscale-bmp
                lum = /*blue*/(row[x1++] * 0.11) + /*green*/(row[x1++] * 0.59) + /*red*/(row[x1++] * 0.30);
                if (lum < 0) lum = 0;
                if (lum > 255) lum = 255;
                *tmp = (uint8_t)lum;
            }
        }
        *_size = _fb;
    }

    return image;
}

uint8_t* canvas::pbm_open(const char* fileName, uint32_t* x0, uint32_t* y0, uint32_t* _size)
{
    *x0 = *y0 = *_size = 0;

    bool err = 0;
    char trash[500];
    uint8_t* image = nullptr;
    uint32_t image_size = 0;
    uint32_t dim[2] = { 0x00 };

    FILE* fd;
    //fopen(fileName, "rb");
    fopen_s(&fd, fileName, "rb");

    if (fd != NULL) {
        fseek(fd, 0, SEEK_END);
        size_t file_size = ftell(fd);
        fseek(fd, 0, SEEK_SET);

        if (fgets(trash, 5, fd) != NULL) {
            if (!strcmp(trash, "P4\n")) {
                do {
                    if (fgets(trash, 500, fd) == NULL) {
                        err = 1;
                        break;
                    }
                } while (trash[0] == '#');
                int i = 0; 
                char* pch = nullptr, * pchb = nullptr;
                //pch = strtok_s(trash, " \n");
                pch = strtok_s(trash, " \n", &pchb);
                while (pch != NULL) {
                    dim[i++] = atoi(pch);
                    //pch = strtok_s(NULL, " \n");
                    pch = strtok_s(NULL, " \n", &pchb);
                }
            }
        }

        if (!err) {
            image_size = (uint32_t)(file_size - ftell(fd));
            image = new uint8_t[image_size];
            if (image != nullptr) {
                fread(image, sizeof(uint8_t), image_size, fd);
                if (ferror(fd) == 0) {
                    if (ferror(fd)) {
                        delete[] image;
                        image = nullptr;
                    } else {
                        *y0 = dim[1];
                        *x0 = dim[0];
                        *_size = image_size;
                    }
                }
            }
        }
        fclose(fd);
    }

    return image;
}

bool canvas::import_jbg(const char* fileName)
{
    bool err = 0;
    uint32_t x = 0, y = 0, s = 0;
    uint8_t* image = jbg_open(fileName, &x, &y, &s);

    if (image != nullptr) {
        uint8_t* tmp;
        uint32_t x1 = s / y;
        uint32_t q, _y, _x = 0;

        if ((err |= create(x, y, 0)) != 1) {
            for (_y = 0; _y < y; _y++) {
                q = x1 * _y;
                tmp = image + q;

                uint8_t bit = 0;
                for (_x = 0; _x < x; _x++) {
                    if (BIT_CHECK(tmp[_x / 8], 7-bit))
                        setPixle(_x, _y, 1);
                    bit = (bit + 1) % 8;
                }
            }
        } 
        free(image);
    }

    return err;
}

bool canvas::import_24bit(const char* fileName, DITHER type, int b_level, int c_level)
{
    float* tmp;
    bool err = 0;
    uint32_t x = 0, y = 0, s = 0, q = 0;
    uint32_t _x = 0, _y = 0, _yi;
    uint8_t* image = bmp_open(fileName, &x, &y, &s);

    if (b_level != 0) err |= adj_brightness(image, x, y, b_level);
    if (c_level != 0) err |= adj_contrast(image, x, y, c_level);

    if (image != nullptr) {
        float* _d = new float[s];
        if (_d != nullptr) {
            for (uint32_t i = 0; i < s; i++)
                _d[i] = (float)image[i];
            delete[] image;
            img _img = { _d, x, y };
            if ((err |= dither(&_img, type)) != 1) {
                _yi = y - 1;
                if ((err |= create(x, y, 0)) != 1) {
                    for (_y = 0; _y < y; _y++) {
                        for (_x = 0; _x < x; _x++) {
                            q = ((((_yi)-_y) * x)) + _x;
                            tmp = _img.__img + q;
                            s = (uint32_t)*tmp;
                            if (s == 0) 
                                setPixle(_x, _y, 1);
                        }
                    }
                }
            }
            delete[] _d;
        }
        else delete[] image;
    }
    else return 1;

    return err;
}

bool canvas::import_pbm(const char* fileName)
{
    bool err = 0;
    uint32_t x = 0, y = 0, s = 0;
    uint8_t* image_pbm = pbm_open(fileName, &x, &y, &s);

    if (image_pbm != nullptr) {
        uint32_t x1 = s / y;
        uint32_t q, _y, _x = 0;
        uint8_t* tmp;

        if ((err |= create(x, y, 0)) != 1) {
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
        delete[] image_pbm;
    }

    return err;
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

/* found here: https://eab.abime.net/showthread.php?t=29492 */
bool canvas::rot_calc(int32_t* x0, int32_t* y0, uint32_t x_size, uint32_t y_size, int angle)
{
    float a = (float)(angle * (3.14159 / 180.0)); //in rad

    float center_x = (float)(x_size / 2);
    float center_y = (float)(y_size / 2);
    
    int32_t xp = (int32_t)((*x0 - center_x) * cos(a) - (*y0 - center_y) * sin(a) /*+ center_x*/);
    int32_t yp = (int32_t)((*x0 - center_x) * sin(a) + (*y0 - center_y) * cos(a) /*+ center_y*/);

    *x0 = xp;
    *y0 = yp;

    return 0;
}

/* found here: https://eab.abime.net/showthread.php?t=29492 */
bool canvas::rotate_full(int angle)
{
    bool err = 0;
    canvas temp;

    if (ptr != nullptr) {

        /* min-max search */
        int32_t MAX_X = 0, MAX_Y = 0;
        int32_t MIN_X = 0, MIN_Y = 0;

        int32_t __x[4] = { 0,            0, (int32_t) _x, (int32_t) _x };
        int32_t __y[4] = { 0, (int32_t) _y,            0, (int32_t) _y };

        for (int i = 0; i < 4; i++) {
            rot_calc(&__x[i], &__y[i], _x, _y, angle);
            if (__x[i] < MIN_X) MIN_X = __x[i];
            if (__y[i] < MIN_Y) MIN_Y = __y[i];
            if (__x[i] > MAX_X) MAX_X = __x[i];
            if (__y[i] > MAX_Y) MAX_Y = __y[i];
        }

        MIN_X = abs(MIN_X);
        MIN_Y = abs(MIN_Y);
        MAX_X += MIN_X;
        MAX_Y += MIN_Y;
        
        if (temp.create(MAX_X, MAX_Y, 0) == 1)
            return 1;

        int32_t xs = 0;
        int32_t ys = 0;

        for (uint32_t y = 0; y < _y; y++) {
            for (uint32_t x = 0; x < _x; x++) {
                xs = x;
                ys = y;
                rot_calc(&xs, &ys, _x, _y, angle);
                temp.setPixle(xs + MIN_X, ys + MIN_Y, getPixle(x, y));
            }
        }

        delete[] ptr;
        ptr = nullptr;
        err |= create(temp.get_pointer(), MAX_X, MAX_Y, 0);
    }

    return err;
}

bool canvas::dither(img* image, DITHER type)
{
    bool err = 0;
    switch (type)
    {
    case DITHER::FloydSteinberg:
        err |= floydSteinberg(image);       break;
    case DITHER::Stucki:
        err |=  stucki(image);              break;
    case DITHER::Jarvis:
        err |= jarvis(image);               break;
    case DITHER::Atkinson:
        err |= atkinson(image);             break;
    case DITHER::Bayer_2x2:
        err |= bayer(image, 0);             break;
    case DITHER::Bayer_4x4:
        err |= bayer(image, 1);             break;
    case DITHER::Bayer_8x8:
        err |= bayer(image, 2);             break;
    case DITHER::Bayer_16x16:
        err |= bayer(image, 3);             break;
    case DITHER::Cluster:
        err |= cluster(image);              break;
    case DITHER::Threshold:
        err |= threshold(image, 256 / 2);   break;
    default:
        err = 1;
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

bool canvas::adj_brightness(uint8_t* ptr, uint32_t x0, uint32_t y0, int val)
{
    int pix;
    uint8_t* p;
    uint32_t y = 0, x = 0;

    if (ptr != nullptr) {
        for (y = 0; y < y0; y++) {
            for (x = 0; x < x0; x++) {
                p = &ptr[(x * y0) + y];
                pix = *p + val;
                if (pix > 255) pix = 255;
                if (pix < 0) pix = 0;
                *p = (uint8_t)pix;
            }
        }
        return 0;
    }
    return 1;
}

bool canvas::adj_contrast(uint8_t* ptr, uint32_t x0, uint32_t y0, int val)
{
    uint8_t* p;
    uint32_t y = 0, x = 0;
    float _P = 0.0, F = (259.0F * (val + 255.0F)) / (255.0F * (259.0F - val));

    if (ptr != nullptr) {
        for (y = 0; y < y0; y++) {
            for (x = 0; x < x0; x++) {
                p = &ptr[(x * y0) + y];
                _P = (F * (*p - 128.0F)) + 128.0F;
                if (_P > 255.0F) _P = 255.0F;
                if (_P < 0.0F) _P = 0.0F;
                *p = (uint8_t)_P;
            }
        }
        return 0;
    }
    return 1;
}

bool canvas::scale(float x0, float y0)
{
    bool err = 0;
    canvas temp;

    if (x0 <= 0 || y0 <= 0)
        return 1;

    if (ptr != nullptr) {
        uint32_t newX = (int32_t)(_x * x0);
        uint32_t newY = (int32_t)(_y * y0);

        if (temp.create(newX, newY, 0) == 1)
            return 1;

        for (uint32_t y = 0; y < newY; y++)
            for (uint32_t x = 0; x < newX; x++)
                temp.setPixle(x, y, getPixle((uint32_t)(x/x0), (uint32_t)(y/y0)));

        delete[] ptr;
        ptr = nullptr;
        err |= create(temp.get_pointer(), newX, newY, 0);
    }

    return err;
}


/*******  JUST A TEST, BUGGY CODE ********/
bool canvas::TEST(int head_size)
{
    return 0;
}