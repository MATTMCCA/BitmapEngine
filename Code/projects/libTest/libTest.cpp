/*
    ***************************************************************************************
    MJM -2021-

    ---------------------------------------------------------------------------------------

    =======================================================================================

        Notes: Very little error handling, its a demo?

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

#include <string>
#include <chrono>
#include <cstdio>
#include <cstdint>
#include <cctype>
#include <ctime>

#include "canvas.hpp"
#include "font.hpp"
#include "zpl.hpp"

using namespace std::chrono;

const char ARIAL_FONT[] = "C:\\Windows\\Fonts\\arial.ttf";
const char TEST_DIR[] = "A:\\Users\\Matt\\Pictures\\TRASH\\libtest\\";
const char TEST_IMG[] = "A:\\Users\\Matt\\Pictures\\TRASH\\Lena.bmp";
//const char TEST_IMG[] = "A:\\Users\\Matt\\Pictures\\TRASH\\logo.bmp";
//const char TEST_IMG[] = "A:\\Users\\Matt\\Pictures\\TRASH\\hi.bmp";

const int DPI = 300;

std::string JBIG_TESTIMG;
std::string PBM_TESTIMG;

int DEBUG(void);

bool save_bmp(canvas* ptr, const char* dir, int cnt);
bool save_pbm(canvas* ptr, const char* dir, int cnt);
bool save_jbg(canvas* ptr, const char* dir, int cnt);
bool save_ba(canvas* ptr, const char* dir, int cnt);
bool save_zpl(canvas* ptr, const char* dir, int cnt);

void print_pass_fail(const char* testname, bool err);

bool canvas_noninv(const char* dir, int cnt);
bool canvas_inv(const char* dir, int cnt);
bool canvas_fill(const char* dir, int cnt);
bool canvas_setPixle(const char* dir, int cnt);
bool canvas_setPixle_rand(const char* dir, int cnt);
bool canvas_Hline(const char* dir, int cnt);
bool canvas_Vline(const char* dir, int cnt);
bool canvas_box(const char* dir, int cnt);
bool canvas_box_fill(const char* dir, int cnt);
bool canvas_box_fill_inv(const char* dir, int cnt);
bool canvas_import_bitmap_Threshold(const char* dir, const char* bmp, int cnt);
bool canvas_import_bitmap_FloydSteinberg(const char* dir, const char* bmp, int cnt);
bool canvas_import_bitmap_Stucki(const char* dir, const char* bmp, int cnt);
bool canvas_import_bitmap_Jarvis(const char* dir, const char* bmp, int cnt);
bool canvas_import_bitmap_Atkinson(const char* dir, const char* bmp, int cnt);
bool canvas_import_bitmap_Bayer_2x2(const char* dir, const char* bmp, int cnt);
bool canvas_import_bitmap_Bayer_4x4(const char* dir, const char* bmp, int cnt);
bool canvas_import_bitmap_Bayer_8x8(const char* dir, const char* bmp, int cnt);
bool canvas_import_bitmap_Bayer_16x16(const char* dir, const char* bmp, int cnt);
bool canvas_import_bitmap_Cluster_16x16(const char* dir, const char* bmp, int cnt);
bool canvas_rotate_90(const char* dir, const char* bmp, int cnt);
bool canvas_rotate_180(const char* dir, const char* bmp, int cnt);
bool canvas_rotate_270(const char* dir, const char* bmp, int cnt);
bool canvas_rotate_270_inv(const char* dir, const char* bmp, int cnt);
bool font_noninv(const char* dir, const char* fnt, int cnt);
bool font_inv(const char* dir, const char* fnt, int cnt);
bool font_size(const char* dir, const char* fnt, int cnt);
bool font_rot(const char* dir, const char* fnt, int cnt);
bool font_offset(const char* dir, const char* fnt, int cnt);
bool font_huge(const char* dir, const char* fnt, int cnt);
bool font_small(const char* dir, const char* fnt, int cnt);
bool sprite_overlay_no_alpha(const char* dir, const char* fnt, const char* bmp, int cnt);
bool sprite_overlay_alpha(const char* dir, const char* fnt, const char* bmp, int cnt);
bool canvas_mirrorH(const char* dir, const char* bmp, int cnt);
bool canvas_mirrorV(const char* dir, const char* bmp, int cnt);
bool image_save_PBM(const char* dir, const char* bmp, int cnt);
/* https://filext.com/file-extension/JBG */
/* http://manpages.ubuntu.com/manpages/bionic/man1/jbgtopbm.1.html */
bool image_save_JBIG(const char* dir, const char* bmp, int cnt);          //JBIG compressed bitmap
bool import_jbg_file(char* dir, const char* bmp, int cnt);
bool import_pbm_file(char* dir, const char* bmp, int cnt);
bool image_save_xbm(const char* dir, const char* bmp, int cnt);
bool image_save_zpl(const char* dir, const char* bmp, int cnt);
bool image_brightness_adj(const char* dir, const char* bmp, int cnt);
bool image_contrast_adj(const char* dir, const char* bmp, int cnt);
bool canvas_scale_grow(const char* dir, const char* bmp, int cnt);
bool canvas_scale_shrink(const char* dir, const char* bmp, int cnt);
bool canvas_scale_grow_width(const char* dir, const char* bmp, int cnt);
bool canvas_rotate_15(const char* dir, const char* bmp, int cnt);

int main(int argc, char* argv[])
{
    /*
    DEBUG();
    return 0;
    */

    char* output_dir = (char*)TEST_DIR;
    char* tst_img = (char*)TEST_IMG;
    auto start = high_resolution_clock::now();

    if (argc > 2) {
        tst_img = argv[1];
        output_dir = argv[2];
    }
    printf("output_dir: %s\n", output_dir); //debugging
    printf("test_image: %s\n", tst_img); //debugging

    /* Running Tests */
    int tc = 0;

    canvas_noninv(output_dir, tc++);
    canvas_inv(output_dir, tc++);
    canvas_fill(output_dir, tc++);
    canvas_setPixle(output_dir, tc++);
    canvas_setPixle_rand(output_dir, tc++);
    canvas_Hline(output_dir, tc++);
    canvas_Vline(output_dir, tc++);
    canvas_box(output_dir, tc++);
    canvas_box_fill(output_dir, tc++);
    canvas_box_fill_inv(output_dir, tc++);
    canvas_import_bitmap_Threshold(output_dir, tst_img, tc++);
    canvas_import_bitmap_FloydSteinberg(output_dir, tst_img, tc++);
    canvas_import_bitmap_Stucki(output_dir, tst_img, tc++);
    canvas_import_bitmap_Jarvis(output_dir, tst_img, tc++);
    canvas_import_bitmap_Atkinson(output_dir, tst_img, tc++);
    canvas_import_bitmap_Bayer_2x2(output_dir, tst_img, tc++);
    canvas_import_bitmap_Bayer_4x4(output_dir, tst_img, tc++);
    canvas_import_bitmap_Bayer_8x8(output_dir, tst_img, tc++);
    canvas_import_bitmap_Bayer_16x16(output_dir, tst_img, tc++);
    canvas_import_bitmap_Cluster_16x16(output_dir, tst_img, tc++);
    canvas_rotate_90(output_dir, tst_img, tc++);
    canvas_rotate_180(output_dir, tst_img, tc++);
    canvas_rotate_270(output_dir, tst_img, tc++);
    canvas_rotate_270_inv(output_dir, tst_img, tc++);
    canvas_mirrorH(output_dir, tst_img, tc++);
    canvas_mirrorV(output_dir, tst_img, tc++);
    font_noninv(output_dir, ARIAL_FONT, tc++);
    font_inv(output_dir, ARIAL_FONT, tc++);
    font_size(output_dir, ARIAL_FONT, tc++);
    font_rot(output_dir, ARIAL_FONT, tc++);
    font_offset(output_dir, ARIAL_FONT, tc++);
    //font_huge(output_dir, ARIAL_FONT, tc++);
    //font_small(output_dir, ARIAL_FONT, tc++);
    sprite_overlay_no_alpha(output_dir, ARIAL_FONT, tst_img, tc++);
    sprite_overlay_alpha(output_dir, ARIAL_FONT, tst_img, tc++);
    image_save_PBM(output_dir, tst_img, tc++);
    image_save_JBIG(output_dir, tst_img, tc++);
    import_jbg_file(output_dir, JBIG_TESTIMG.c_str(), tc++);
    import_pbm_file(output_dir, PBM_TESTIMG.c_str(), tc++);
    image_save_xbm(output_dir, tst_img, tc++);
    image_save_zpl(output_dir, tst_img, tc++);
    image_brightness_adj(output_dir, tst_img, tc++);
    image_contrast_adj(output_dir, tst_img, tc++);
    canvas_rotate_15(output_dir, tst_img, tc++);
    canvas_scale_grow(output_dir, tst_img, tc++);
    canvas_scale_shrink(output_dir, tst_img, tc++);
    canvas_scale_grow_width(output_dir, tst_img, tc++);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    printf("Took: %I64d milliseconds\n", duration.count());

    return 0;
}

bool save_bmp(canvas* ptr, const char* dir, int cnt)
{
    std::string output = std::string(dir) + std::to_string(cnt) + std::string(".bmp");
    printf("%s\t-> ", std::string(std::to_string(cnt) + std::string(".bmp")).c_str());
    return ptr->saveBMP(output.c_str(), DPI);
}

bool save_pbm(canvas* ptr, const char* dir, int cnt)
{
    std::string output = std::string(dir) + std::to_string(cnt) + std::string(".pbm");
    printf("%s\t-> ", std::string(std::to_string(cnt) + std::string(".pbm")).c_str());
    return ptr->savePBM(output.c_str());
}

bool save_jbg(canvas* ptr, const char* dir, int cnt)
{
    std::string output = std::string(dir) + std::to_string(cnt) + std::string(".jbg");
    printf("%s\t-> ", std::string(std::to_string(cnt) + std::string(".jbg")).c_str());
    return ptr->saveJBG(output.c_str());
}

bool save_xbm(canvas* ptr, const char* dir, int cnt)
{
    std::string output = std::string(dir) + std::to_string(cnt) + std::string(".xbm");
    printf("%s\t-> ", std::string(std::to_string(cnt) + std::string(".xbm")).c_str());
    return ptr->saveXBM(output.c_str(), "myImage");
}

bool save_zpl(canvas* ptr, const char* dir, int cnt)
{
    PARM thisjob = {
        {'A', 0},       /* ^MNa         */
        {0,0},          /* ^LHx,y       */
        {'T', 'N'},     /* ^MMa,b       */
        {4 * 300},      /* ^PWa         */
        {0},            /* ^LSa         */
        {'N'},          /* ^POa         */
        {6 * 300},      /* ^LLy         */
        {8,0,2},        /* ^PRp,s,b     */
        {8},            /* ~SD##        */
        {0,0,0},        /* ^FOx,y,z     */
        {1,0,0,'N','Y'} /* ^PQq,p,r,o,e */
    };

    zpl job(thisjob);
    bool err = 0;
    err |= job.add_graphic(ptr->get_pointer(), ptr->get_x(), ptr->get_y());
    err |= job.generate_format();
 
    std::string output = std::string(dir) + std::to_string(cnt) + std::string(".zpl");
    printf("%s\t-> ", std::string(std::to_string(cnt) + std::string(".zpl")).c_str());
    return err | job.save_format(output.c_str());
}

void print_pass_fail(const char* testname, bool err)
{
    if (err)
        printf("FAILED: %s\n", testname);
    else
        printf("PASSED: %s\n", testname);
}

bool canvas_noninv(const char* dir, int cnt)
{
    bool err = 0;
    canvas c;
    err |= c.create(200, 200, 0);
    err |= save_bmp(&c, dir, cnt);
    print_pass_fail("Canvas (non inverted)", err);
    return err;
}

bool canvas_inv(const char* dir, int cnt)
{
    bool err = 0;
    canvas c;
    err |= c.create(200, 200, 1);
    err |= save_bmp(&c, dir, cnt);
    print_pass_fail("Canvas (inverted)", err);
    return err;
}

bool canvas_fill(const char* dir, int cnt)
{
    bool err = 0;
    canvas c;
    err |= c.create(200, 200, 0);
    err |= c.fill(1);
    err |= save_bmp(&c, dir, cnt);
    print_pass_fail("Canvas (fill)", err);
    return err;
}

bool canvas_setPixle(const char* dir, int cnt)
{
    bool err = 0;
    canvas c;
    err |= c.create(200, 200, 0);
    
    bool flop = 0;
    for (int y = 0; y < 200; y++) {
        for (int x = 0; x < 200; x++)
            err |= c.setPixle(x, y, (flop = !flop));
        flop = !flop;
    }

    err |= save_bmp(&c, dir, cnt);
    print_pass_fail("Canvas (setPixle)", err);
    return err;
}

bool canvas_setPixle_rand(const char* dir, int cnt)
{
    bool err = 0;
    canvas c;
    err |= c.create(200, 200, 0);

    for (int i = 0; i < 500; i++) {
        err |= c.setPixle(rand() % 200, rand() % 200, 1);
    }

    err |= save_bmp(&c, dir, cnt);
    print_pass_fail("Canvas (setPixle_random)", err);
    return err;
}

bool canvas_Hline(const char* dir, int cnt)
{
    bool err = 0;
    canvas c;
    err |= c.create(200, 200, 0);

    for (int i = 0; i < 10; i++) {
        err |= c.drawHorizontalLine(rand() % 200, rand() % 200, rand() % 200, rand() % 20, 1);
    }

    err |= save_bmp(&c, dir, cnt);
    print_pass_fail("Canvas (drawHorizontalLine)", err);
    return err;
}

bool canvas_Vline(const char* dir, int cnt)
{
    bool err = 0;
    canvas c;
    err |= c.create(200, 200, 0);

    for (int i = 0; i < 10; i++) {
        err |= c.drawVerticalLine(rand() % 200, rand() % 200, rand() % 200, rand() % 20, 1);
    }

    err |= save_bmp(&c, dir, cnt);
    print_pass_fail("Canvas (drawVerticalLine)", err);
    return err;
}

bool canvas_box(const char* dir, int cnt)
{
    bool err = 0;
    canvas c;
    err |= c.create(200, 200, 0);

    for (int i = 1; i < 10; i ++) {
        int h = i * 5;
        err |= c.drawBox(h, h, 200 - h*4, 200 - h*4, 2, 1);
    }

    err |= save_bmp(&c, dir, cnt);
    print_pass_fail("Canvas (box)", err);
    return err;
}

bool canvas_box_fill(const char* dir, int cnt)
{
    bool err = 0;
    canvas c;
    err |= c.create(200, 200, 0);

    for (int i = 1; i < 20; i++) 
        err |= c.drawBoxFill(i*4, i*4, 20, 20, i%2);

    err |= save_bmp(&c, dir, cnt);
    print_pass_fail("Canvas (box_fill)", err);
    return err;
}

bool canvas_box_fill_inv(const char* dir, int cnt)
{
    bool err = 0;
    canvas c;
    err |= c.create(200, 200, 0);

    for (int i = 1; i < 20; i++)
        err |= c.drawBoxFill(i * 4, i * 4, 20, 20, i % 2);

    err |= c.invert(1);
    err |= save_bmp(&c, dir, cnt);
    print_pass_fail("Canvas (box_fill_invert)", err);
    return err;
}

bool canvas_import_bitmap_Threshold(const char* dir, const char* bmp, int cnt)
{
    bool err = 0;
    canvas c;
    err |= c.import_24bit(bmp, DITHER::Threshold);

    err |= save_bmp(&c, dir, cnt);
    print_pass_fail("Canvas (bitmap_import_Threshold)", err);
    return err;
}

bool canvas_import_bitmap_FloydSteinberg(const char* dir, const char* bmp, int cnt)
{
    bool err = 0;
    canvas c;
    err |= c.import_24bit(bmp, DITHER::FloydSteinberg);

    err |= save_bmp(&c, dir, cnt);
    print_pass_fail("Canvas (bitmap_import_FloydSteinberg)", err);
    return err;
}

bool canvas_import_bitmap_Stucki(const char* dir, const char* bmp, int cnt)
{
    bool err = 0;
    canvas c;
    err |= c.import_24bit(bmp, DITHER::Stucki);

    err |= save_bmp(&c, dir, cnt);
    print_pass_fail("Canvas (bitmap_import_Stucki)", err);
    return err;
}

bool canvas_import_bitmap_Jarvis(const char* dir, const char* bmp, int cnt)
{
    bool err = 0;
    canvas c;
    err |= c.import_24bit(bmp, DITHER::Jarvis);

    err |= save_bmp(&c, dir, cnt);
    print_pass_fail("Canvas (bitmap_import_Jarvis)", err);
    return err;
}

bool canvas_import_bitmap_Atkinson(const char* dir, const char* bmp, int cnt)
{
    bool err = 0;
    canvas c;
    err |= c.import_24bit(bmp, DITHER::Atkinson);

    err |= save_bmp(&c, dir, cnt);
    print_pass_fail("Canvas (bitmap_import_Atkinson)", err);
    return err;
}

bool canvas_import_bitmap_Bayer_2x2(const char* dir, const char* bmp, int cnt)
{
    bool err = 0;
    canvas c;
    err |= c.import_24bit(bmp, DITHER::Bayer_2x2);

    err |= save_bmp(&c, dir, cnt);
    print_pass_fail("Canvas (bitmap_import_Bayer_2x2)", err);
    return err;
}

bool canvas_import_bitmap_Bayer_4x4(const char* dir, const char* bmp, int cnt)
{
    bool err = 0;
    canvas c;
    err |= c.import_24bit(bmp, DITHER::Bayer_4x4);

    err |= save_bmp(&c, dir, cnt);
    print_pass_fail("Canvas (bitmap_import_Bayer_4x4)", err);
    return err;
}

bool canvas_import_bitmap_Bayer_8x8(const char* dir, const char* bmp, int cnt)
{
    bool err = 0;
    canvas c;
    err |= c.import_24bit(bmp, DITHER::Bayer_8x8);

    err |= save_bmp(&c, dir, cnt);
    print_pass_fail("Canvas (bitmap_import_Bayer_8x8)", err);
    return err;
}

bool canvas_import_bitmap_Bayer_16x16(const char* dir, const char* bmp, int cnt)
{
    bool err = 0;
    canvas c;
    err |= c.import_24bit(bmp, DITHER::Bayer_16x16);

    err |= save_bmp(&c, dir, cnt);
    print_pass_fail("Canvas (bitmap_import_Bayer_16x16)", err);
    return err;
}

bool canvas_import_bitmap_Cluster_16x16(const char* dir, const char* bmp, int cnt)
{
    bool err = 0;
    canvas c;
    err |= c.import_24bit(bmp, DITHER::Cluster);

    err |= save_bmp(&c, dir, cnt);
    print_pass_fail("Canvas (bitmap_import_Cluster)", err);
    return err;
}

bool canvas_rotate_90(const char* dir, const char* bmp, int cnt)
{
    bool err = 0;
    canvas c;
    err |= c.import_24bit(bmp, DITHER::Jarvis);
    c.rotate(DEGREE::ROT_90);

    err |= save_bmp(&c, dir, cnt);
    print_pass_fail("Canvas (bitmap_rot_90)", err);
    return err;
}

bool canvas_rotate_180(const char* dir, const char* bmp, int cnt)
{
    bool err = 0;
    canvas c;
    err |= c.import_24bit(bmp, DITHER::Jarvis);
    c.rotate(DEGREE::ROT_180);

    err |= save_bmp(&c, dir, cnt);
    print_pass_fail("Canvas (bitmap_rot_180)", err);
    return err;
}

bool canvas_rotate_270(const char* dir, const char* bmp, int cnt)
{
    bool err = 0;
    canvas c;
    err |= c.import_24bit(bmp, DITHER::Jarvis);
    c.rotate(DEGREE::ROT_270);

    err |= save_bmp(&c, dir, cnt);
    print_pass_fail("Canvas (bitmap_rot_270)", err);
    return err;
}

bool canvas_rotate_270_inv(const char* dir, const char* bmp, int cnt)
{
    bool err = 0;
    canvas c;
    err |= c.import_24bit(bmp, DITHER::Jarvis);
    c.rotate(DEGREE::ROT_270);
    c.invert(1);

    err |= save_bmp(&c, dir, cnt);
    print_pass_fail("Canvas (bitmap_rot_270_inv)", err);
    return err;
}

bool font_noninv(const char* dir, const char* fnt, int cnt)
{
    bool err = 0;
    canvas c;
    font f;
    err |= f.create(fnt, 20, DPI);
    err |= f.writeCanvas(&c, "TESTSTRING", 0, 0);

    err |= save_bmp(&c, dir, cnt);
    print_pass_fail("font (non_inverting)", err);
    return err;
}

bool font_inv(const char* dir, const char* fnt, int cnt)
{
    bool err = 0;
    canvas c;
    font f;
    err |= f.create(fnt, 20, DPI);
    err |= f.writeCanvas(&c, "TESTSTRING", 0, 0);
    c.invert(1);

    err |= save_bmp(&c, dir, cnt);
    print_pass_fail("font (inverting)", err);
    return err;
}

bool font_size(const char* dir, const char* fnt, int cnt)
{
    bool err = 0;
    canvas c;
    err |= c.create(350, 350, 0);

    std::vector<font> font_list;
    font_list.resize(15);
    for (int i = 0; i < 15; i ++) {
        err |= font_list[i].create(fnt, (i + 1) * 2, DPI);
        err |= font_list[i].writeCanvas(&c, "String", 5, 20 + (i*i));
    }

    err |= save_bmp(&c, dir, cnt);
    print_pass_fail("font (size)", err);
    return err;
}

bool font_rot(const char* dir, const char* fnt, int cnt)
{
    bool err = 0;
    font f_10pt;

    canvas c;
    canvas rot_0;
    canvas rot_90;
    canvas rot_180;
    canvas rot_270;

    err |= c.create(500, 500, 0);
    err |= f_10pt.create(fnt, 10, DPI);
    err |= f_10pt.writeCanvas(&rot_0, "TEST_STRING!", 0, 0);

    rot_90 = rot_180 = rot_270 = rot_0;

    err |= rot_90.rotate(DEGREE::ROT_90);
    err |= rot_180.rotate(DEGREE::ROT_180);
    err |= rot_270.rotate(DEGREE::ROT_270);

    err |= c.addSprite(&rot_0, (c.get_x() - rot_180.get_x()) / 2, 0, 0);
    err |= c.addSprite(&rot_90, c.get_x() - rot_90.get_x(), (c.get_y() - rot_90.get_y()) / 2, 0);
    err |= c.addSprite(&rot_180, (c.get_x() - rot_180.get_x()) / 2, c.get_y() - rot_180.get_y(), 0);
    err |= c.addSprite(&rot_270, 0, (c.get_y() - rot_270.get_y()) / 2, 0);
    
    err |= save_bmp(&c, dir, cnt);
    print_pass_fail("font (rotate)", err);
    return err;
}

bool font_offset(const char* dir, const char* fnt, int cnt)
{
    bool err = 0;
    font f_10pt;

    canvas c;
    err |= c.create(450, 200, 0);
    err |= f_10pt.create(fnt, 10, DPI);
    err |= f_10pt.writeCanvas(&c, "TEST_STRING!", 5, 10);
    err |= f_10pt.changeCharOffset(5, 0);
    err |= f_10pt.writeCanvas(&c, "TEST_STRING!", 5, 60);
    err |= f_10pt.changeCharOffset(-10, 0);
    err |= f_10pt.writeCanvas(&c, "TEST_STRING!", 5, 110);

    err |= save_bmp(&c, dir, cnt);
    print_pass_fail("font (offsete)", err);
    return err;
}

bool font_huge(const char* dir, const char* fnt, int cnt)
{
    bool err = 0;
    font f_10pt;
    canvas c;
    err |= f_10pt.create(fnt, 500, DPI);
    err |= f_10pt.writeCanvas(&c, "HUGE!", 0, 0);
    err |= save_bmp(&c, dir, cnt);
    print_pass_fail("font (huge)", err);
    return err;
}

bool font_small(const char* dir, const char* fnt, int cnt)
{
    bool err = 0;
    font f_10pt;
    canvas c;
    err |= f_10pt.create(fnt, 4, DPI);
    err |= f_10pt.writeCanvas(&c, "small...", 0, 0);
    err |= save_bmp(&c, dir, cnt);
    print_pass_fail("font (small)", err);
    return err;
}

bool sprite_overlay_no_alpha(const char* dir, const char* fnt, const char* bmp, int cnt)
{
    bool err = 0;
    font f_10pt;
    canvas c;
    canvas f_c;

    err |= c.import_24bit(bmp, DITHER::Jarvis);
    err |= f_10pt.create(fnt, 10, DPI);
    err |= f_10pt.writeCanvas(&f_c, "TEST_STRING!", 0, 0);

    err |= c.addSprite(&f_c, 0, 0, 0);
    err |= save_bmp(&c, dir, cnt);
    print_pass_fail("font (overlay_no_alpha)", err);
    return err;
}

bool sprite_overlay_alpha(const char* dir, const char* fnt, const char* bmp, int cnt)
{
    bool err = 0;
    font f_10pt;
    canvas c;
    canvas f_c;

    err |= c.import_24bit(bmp, DITHER::Jarvis);
    err |= f_10pt.create(fnt, 10, DPI);
    err |= f_10pt.writeCanvas(&f_c, "TEST_STRING!", 0, 0);

    err |= c.addSprite(&f_c, 0, 0, 1); //change alpha to 1
    err |= save_bmp(&c, dir, cnt);
    print_pass_fail("font (overlay_alpha)", err);
    return err;
}

bool canvas_mirrorH(const char* dir, const char* bmp, int cnt)
{
    bool err = 0;
    canvas c;
    err |= c.import_24bit(bmp, DITHER::Jarvis);
    err |= c.mirror(MIRROR::Horizontal);

    err |= save_bmp(&c, dir, cnt);
    print_pass_fail("Canvas (MIRROR::Horizontal)", err);
    return err;
}

bool canvas_mirrorV(const char* dir, const char* bmp, int cnt)
{
    bool err = 0;
    canvas c;
    err |= c.import_24bit(bmp, DITHER::Jarvis);
    err |= c.mirror(MIRROR::Vertical);

    err |= save_bmp(&c, dir, cnt);
    print_pass_fail("Canvas (MIRROR::Vertical)", err);
    return err;
}

bool image_save_PBM(const char* dir, const char* bmp, int cnt)
{
    bool err = 0;
    canvas c;
    err |= c.import_24bit(bmp, DITHER::Threshold);
    //err |= c.invert(1);

    err |= save_pbm(&c, dir, cnt);
    print_pass_fail("PBM_TEST", err);

    if (err == 0)
        PBM_TESTIMG = std::string(dir) + std::to_string(cnt) + std::string(".pbm");
    return err;
}

bool image_save_JBIG(const char* dir, const char* bmp, int cnt)
{
    bool err = 0;
    canvas c;
    err |= c.import_24bit(bmp, DITHER::Threshold);

    err |= save_jbg(&c, dir, cnt);
    print_pass_fail("JBG_TEST", err);

    if (err == 0)
        JBIG_TESTIMG = std::string(dir) + std::to_string(cnt) + std::string(".jbg");
    return err;
}

bool import_jbg_file(char* dir, const char* bmp, int cnt)
{
    bool err = 0;
    canvas c;
    err |= c.import_jbg(bmp);

    err |= save_bmp(&c, dir, cnt);
    print_pass_fail("jbg_import_export", err);
    return err;
}

bool import_pbm_file(char* dir, const char* bmp, int cnt)
{
    bool err = 0;
    canvas c;
    err |= c.import_pbm(bmp);

    err |= save_bmp(&c, dir, cnt);
    print_pass_fail("pbm_import_export", err);
    return err;
}

bool image_save_xbm(const char* dir, const char* bmp, int cnt)
{
    bool err = 0;
    canvas c;

    err |= c.import_24bit(bmp, DITHER::Threshold);

    err |= save_xbm(&c, dir, cnt);
    print_pass_fail("XBM_TEST", err);
    return err;
}

bool image_save_zpl(const char* dir, const char* bmp, int cnt)
{
    bool err = 0;
    canvas c;
    err |= c.import_24bit(bmp, DITHER::Threshold);
    err |= save_zpl(&c, dir, cnt);
    print_pass_fail("ZPL_TEST", err);
    return err;
}

bool image_brightness_adj(const char* dir, const char* bmp, int cnt)
{
    bool err = 0;
    canvas c;
    err |= c.import_24bit(bmp, DITHER::Jarvis, -50, 0);

    err |= save_bmp(&c, dir, cnt);
    print_pass_fail("image (brightness - 50)", err);
    return err;
}

bool image_contrast_adj(const char* dir, const char* bmp, int cnt)
{
    bool err = 0;
    canvas c;
    err |= c.import_24bit(bmp, DITHER::Jarvis, 0, -50);

    err |= save_bmp(&c, dir, cnt);
    print_pass_fail("image (contrast - 50)", err);
    return err;
}

bool canvas_rotate_15(const char* dir, const char* bmp, int cnt)
{
    bool err = 0;
    canvas c;
    err |= c.import_24bit(bmp, DITHER::Jarvis);
    c.rotate_full(15);

    err |= save_bmp(&c, dir, cnt);
    print_pass_fail("Canvas (bitmap_rot_45)", err);
    return err;
}

bool canvas_scale_grow(const char* dir, const char* bmp, int cnt)
{
    bool err = 0;
    canvas c;
    err |= c.import_24bit(bmp, DITHER::Jarvis);
    c.scale(2.0, 2.0);

    err |= save_bmp(&c, dir, cnt);
    print_pass_fail("Canvas (scale X 2)", err);
    return err;
}

bool canvas_scale_shrink(const char* dir, const char* bmp, int cnt)
{
    bool err = 0;
    canvas c;
    err |= c.import_24bit(bmp, DITHER::Jarvis);
    c.scale(0.5, 0.5);

    err |= save_bmp(&c, dir, cnt);
    print_pass_fail("Canvas (scale X 1/2)", err);
    return err;
}

bool canvas_scale_grow_width(const char* dir, const char* bmp, int cnt)
{
    bool err = 0;
    canvas c;
    err |= c.import_24bit(bmp, DITHER::Jarvis);
    c.scale(2.0, 1);

    err |= save_bmp(&c, dir, cnt);
    print_pass_fail("Canvas (scale)", err);
    return err;
}

int DEBUG(void)
{
    return 0;
}