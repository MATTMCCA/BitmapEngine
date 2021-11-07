/*
    ***************************************************************************************
    MJM -2021-
    zpl
    
    ======================================================================================= 
    
    File: zpl.cpp
    
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

#include "zpl.hpp"

zpl::zpl()
{

}

zpl::zpl(PARM init)
{
    memcpy(&job, &init, sizeof(job));
}

bool zpl::set_media_tracking(char a, int32_t b)
{
    job.media_tracking.media_being_used = a;
    job.media_tracking.bl = b;
    return true;
}

bool zpl::get_media_tracking(char* a, int32_t* b)
{
    *a = job.media_tracking.media_being_used;
    *b = job.media_tracking.bl;
    return true;
}

bool zpl::set_label_home(uint32_t x0, uint32_t y0)
{
    job.lable_home.x = x0;
    job.lable_home.y = y0;
    return true;
}

bool zpl::get_label_home(uint32_t* x0, uint32_t* y0)
{
    *x0 = job.lable_home.x;
    *y0 = job.lable_home.y;
    return true;
}

bool zpl::set_print_mode(char a, char b)
{
    job.print_mode.mode = a;
    job.print_mode.prepeel_select = b;
    return true;
}

bool zpl::get_print_mode(char* a, char* b)
{
    *a = job.print_mode.mode;
    *b = job.print_mode.prepeel_select;
    return true;
}

bool zpl::set_print_width(uint32_t y0)
{
    job.print_width.label_width = y0;
    return true;
}

bool zpl::get_print_width(uint32_t* y0)
{
    *y0 = job.print_width.label_width;
    return true;
}

bool zpl::set_label_shift(int32_t y0)
{
    job.label_shift.shift_left_value = y0;
    return true;
}

bool zpl::get_label_shift(int32_t* y0)
{
    *y0 = job.label_shift.shift_left_value;
    return true;
}

bool zpl::set_print_orientation(char a)
{
    job.print_orientation.value = a;
    return true;
}

bool zpl::get_print_orientation(char* a)
{
    *a = job.print_orientation.value;
    return true;
}

bool zpl::set_label_length(uint32_t y0)
{
    job.label_length.y_axis_pos = y0;
    return true;
}

bool zpl::get_label_length(uint32_t* y0)
{
    *y0 = job.label_length.y_axis_pos;
    return true;
}

bool zpl::set_print_rate(uint32_t p, uint32_t s, uint32_t b)
{
    job.print_rate.print_speed = p;
    job.print_rate.slew_speed = s;
    job.print_rate.backfeed_speed = b;
    return true;
}

bool zpl::get_print_rate(uint32_t* p, uint32_t* s, uint32_t* b)
{
    *p = job.print_rate.print_speed;
    *s = job.print_rate.slew_speed;
    *b = job.print_rate.backfeed_speed;
    return true;
}

bool zpl::set_darkness(uint32_t d)
{
    job.darkness.darkness_setting = d;
    return true;
}

bool zpl::get_darkness(uint32_t* d)
{
    *d = job.darkness.darkness_setting;
    return true;
}

bool zpl::set_field_origin(uint32_t x0, uint32_t y0, uint32_t z0)
{
    job.field_origin.x = x0;
    job.field_origin.y = y0;
    job.field_origin.z = z0;
    return true;
}

bool zpl::get_field_origin(uint32_t* x0, uint32_t* y0, uint32_t* z0)
{
    *x0 = job.field_origin.x;
    *y0 = job.field_origin.y;
    *z0 = job.field_origin.z;
    return true;
}

bool zpl::set_print_quantity(uint32_t q, uint32_t p, uint32_t r, char o, char e)
{
    job.print_quantity.q = q;
    job.print_quantity.p = p;
    job.print_quantity.r = r;
    job.print_quantity.o = o;
    job.print_quantity.e = e;
    return false;
}

bool zpl::get_print_quantity(uint32_t* q, uint32_t* p, uint32_t* r, char* o, char* e)
{
    *q = job.print_quantity.q;
    *p = job.print_quantity.p;
    *r = job.print_quantity.r;
    *o = job.print_quantity.o;
    *e = job.print_quantity.e;
    return false;
}

bool zpl::add_graphic(bool* ptr, uint32_t x0, int32_t y0)
{
    bool err = 0;
    err |= _pack_bool(ptr, x0 * y0, x0);
    err |= _bytes_to_zpl();
    err |= _GFA_prefix(y0);
    err |= _gen_zpl();
    return err;
}

bool zpl::save_zpl(const char* fileName)
{
    if (zpl_data != nullptr) {
       FILE* fd;
        fopen_s(&fd, fileName, "wb");
        if (fd != NULL) {
            fwrite(zpl_data, sizeof(uint8_t), zpl_data_size, fd);
            if ((fclose(fd) != 0) || (ferror(fd)))
                return 1;
            return 0;
        }
        return 1;
    }
    return 1;
}

bool zpl::get_zpl(uint8_t* ptr, uint32_t* len)
{
    if (zpl_data != nullptr) {
        ptr = zpl_data;
        *len = zpl_data_size;
        return false;
    }
    return true;
}

zpl::~zpl()
{
    if (zpl_data != nullptr)
        delete[] zpl_data;
    zpl_data = nullptr;
}

const char* zpl::_freq_to_string(char val, int32_t freq)
{
    std::string tmp;
    if (val != '!' && val != ',') {
        while (freq > 0) {
            for (int i = 38; i >= 0; i--) {
                if ((freq - _frequency[i]) >= 0) {
                    tmp.append(1, _enc[i]);
                    freq -= _frequency[i];
                    break;
                }
            }
        }
    }
    tmp.append(1, val);
    char* ptr = new char[tmp.length() + 1]{ '\0' };
    if (ptr != nullptr)
        memcpy(ptr, tmp.data(), tmp.length());
    else
        return nullptr;

    return ptr;
}

bool zpl::_bytes_to_zpl(void)
{
    bool err = 0;
    uint32_t buffer_size = 0;
    uint8_t* buffer = nullptr;

    if (zpl_data != nullptr) {
        int32_t cnt = 0;
        int32_t _y = zpl_data_size / zpl_row;
        uint8_t* _old = nullptr;
        for (int32_t y = 0; y < _y; y++) {
            uint8_t* line = (zpl_data + (y * zpl_row));
            std::string _row, _tank;
            
            if (_old != nullptr)
                if (memcmp(_old, line, zpl_row) == 0) //if delta == 0
                    _row = std::string(1, ':');

            if (_row.empty()) {
                /* convert to hex */
                for (uint32_t x = 0; x < zpl_row; x++) {
                    uint8_t j = line[x];
                    char h[3] = { '\0' };

                    h[0] = ((j >> 4) & 0x0F);
                    h[1] = (j & 0x0F);
                    h[0] += (h[0] > 9) ? 55 : 48;
                    h[1] += (h[1] > 9) ? 55 : 48;
                    _row.append(h);
                }

                /* prune */
                char _p = NULL;
                if (_row.back() == 'F') _p = 'F';
                if (_row.back() == '0') _p = '0';

                if (_p != NULL) {
                    do {
                        if (!_row.empty()) {
                            _row.pop_back();
                            if (_row.empty())
                                break;
                        }
                    } while (_row.back() == _p);
                    if (_p == 'F') _row.append(1, '!');
                    else           _row.append(1, ',');
                }
                
                /* compress */
                cnt = 0;
                _p = _row.front();
                for (int32_t i = 0; i < (int32_t)_row.length(); i++) {
                    if (_p != _row[i]) {
                        _tank.append(_freq_to_string(_p, cnt));
                        _p = _row[i];
                        cnt = 0;
                    }
                    cnt++;
                }
                if (cnt) _tank.append(_freq_to_string(_p, cnt));
                _row = _tank;
            }

            _old = line;
            uint32_t new_size = buffer_size + (uint32_t)_row.length();
            uint8_t* new_buffer = new uint8_t[new_size];
            if (new_buffer != nullptr) {
                memcpy(new_buffer, buffer, buffer_size);
                memcpy(&new_buffer[buffer_size], _row.data(), _row.length());
                delete[] buffer;
                buffer = new_buffer;
                buffer_size = new_size;
            }
            else err |= 1;
        }

        delete[] zpl_data;
        zpl_data_size = buffer_size;
        zpl_data = buffer;
    }
    else
        err |= 1;

    return err;
}

bool zpl::_pack_bool(bool* ptr, uint32_t _size, uint32_t x0)
{
    bool err = 0;
    if (ptr != nullptr) {

        if (zpl_data == nullptr) {
            delete[] zpl_data;
            zpl_data_size = 0;
        }

        uint32_t image_y = _size / x0;                    /* bool array y */
        uint32_t image_x = (uint32_t)(x0 / 8.0);          /* zpl array x */
        if ((image_x * 8) < (uint32_t)x0)   image_x++;    /* math fix */
        if (image_x == 0)                   image_x = 1;  /* non 0 byte width */

        zpl_data_size = image_x * image_y * sizeof(uint8_t);
        zpl_row = image_x;
        zpl_data = new uint8_t[zpl_data_size]{ 0x00 };

        if (zpl_data != nullptr) {
            memset(zpl_data, 0x00, zpl_data_size);

            bool* in_line;
            uint8_t* out_line;
            uint32_t y = 0, x = 0;

            for (y = 0; y < image_y; y++) {
                in_line = &ptr[(y * x0)];
                out_line = &zpl_data[(y * image_x)];
                for (x = 0; x < x0; x++) {
                    if (in_line[x])
                        BIT_SET(out_line[(x / 8)], (7 - (x % 8)));
                }
            }
        }
        else err |= 1;
    }
    else err |= 1;

    return err;
}

bool zpl::_gen_zpl(void)
{
    bool err = 0;
    uint8_t* tmp = nullptr;

    if (zpl_data != nullptr) {
        _gen_head();
        _gen_foot();

        uint32_t _s = strlen(HEAD) + strlen(FOOT) + zpl_data_size;
        tmp = new uint8_t[_s];
        if (tmp != nullptr) {
            memcpy(tmp, HEAD, strlen(HEAD));
            memcpy(tmp + strlen(HEAD), zpl_data, zpl_data_size);
            memcpy(tmp + strlen(HEAD) + zpl_data_size, FOOT, strlen(FOOT));

            delete[] zpl_data;
            zpl_data = tmp;
            zpl_data_size = _s;
        }
        else err |= 1;
    }
    else err |= 1;

    return err;
}

bool zpl::_gen_head(void)
{
    memset(HEAD, 0x00, BUFFER_SIZE);

    snprintf(HEAD + strlen(HEAD), BUFFER_SIZE - strlen(HEAD),
        "^MN%c,%d\n",
        job.media_tracking.media_being_used,
        job.media_tracking.bl);
    
    snprintf(HEAD + strlen(HEAD), BUFFER_SIZE - strlen(HEAD),
        "^XA\n");

    snprintf(HEAD + strlen(HEAD), BUFFER_SIZE - strlen(HEAD),
        "^LH%d,%d\n",
        job.lable_home.x,
        job.lable_home.y);
    
    snprintf(HEAD + strlen(HEAD), BUFFER_SIZE - strlen(HEAD),
        "^MM%c,%c\n",
        job.print_mode.mode,
        job.print_mode.prepeel_select);

    snprintf(HEAD + strlen(HEAD), BUFFER_SIZE - strlen(HEAD),
        "^PW%d\n",
        job.print_width.label_width);

    snprintf(HEAD + strlen(HEAD), BUFFER_SIZE - strlen(HEAD),
        "^LS%d\n",
        job.label_shift.shift_left_value);

    snprintf(HEAD + strlen(HEAD), BUFFER_SIZE - strlen(HEAD),
        "^PO%c\n",
        job.print_orientation.value);

    snprintf(HEAD + strlen(HEAD), BUFFER_SIZE - strlen(HEAD),
        "^LL%d\n",
        job.label_length.y_axis_pos);

    snprintf(HEAD + strlen(HEAD), BUFFER_SIZE - strlen(HEAD),
        "^PR%d,%d,%d\n",
        job.print_rate.print_speed,
        job.print_rate.slew_speed,
        job.print_rate.backfeed_speed);

    snprintf(HEAD + strlen(HEAD), BUFFER_SIZE - strlen(HEAD),
        "~SD%02d\n",
        job.darkness.darkness_setting);

    snprintf(HEAD + strlen(HEAD), BUFFER_SIZE - strlen(HEAD),
        "^FO%d,%d,%d\n",
        job.field_origin.x,
        job.field_origin.y,
        job.field_origin.z);

    return true;
}

bool zpl::_gen_foot(void)
{
    memset(FOOT, 0x00, BUFFER_SIZE);

    snprintf(FOOT + strlen(FOOT), BUFFER_SIZE - strlen(FOOT),
        "\r\n^PQ%d,%d,%d,%c,%c\n",
        job.print_quantity.q,
        job.print_quantity.p,
        job.print_quantity.r,
        job.print_quantity.o,
        job.print_quantity.e);

    snprintf(FOOT + strlen(FOOT), BUFFER_SIZE - strlen(FOOT),
        "^XZ\r\n");

    return true;
}

bool zpl::_GFA_prefix(int32_t y0)
{
    bool err = 0;
    char _b[512];
    if (zpl_data != nullptr) {
        snprintf(_b, 512, "^GFA,%d,%d,%d, ", zpl_data_size, zpl_row * y0, zpl_row);
        uint8_t* tmp = new uint8_t[strlen(_b) + zpl_data_size];
        if (tmp != nullptr) {
            memcpy(tmp, _b, strlen(_b));
            memcpy(tmp + strlen(_b), zpl_data, zpl_data_size);
            delete[] zpl_data;
            zpl_data_size += strlen(_b);
            zpl_data = tmp;
        }
        else err |= 1;
    }
    else err |= 1;

    return err;
}

