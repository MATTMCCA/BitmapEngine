/*
    ***************************************************************************************
    MJM -2021-

    ---------------------------------------------------------------------------------------

    =======================================================================================

        Notes: <NONE>

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
#include <cstdio>
#include <cstdint>
#include <cctype>
#include <ctime>

#include <process.h>

#include "canvas.hpp"
#include "font.hpp"

#define FONT_ARIAL      "C:\\Windows\\Fonts\\arialbd.ttf"

#define SAVE_AS         "A:\\Users\\Matt\\Pictures\\TRASH\\LLOUT\\"
#define LOGO            "A:\\Users\\Matt\\Pictures\\TRASH\\LL.bmp"
#define LIPS            "A:\\Users\\Matt\\Pictures\\TRASH\\lips.bmp"

enum class game_state { LOST, FIVE_IN_ANY_ROW, FOUR_CORNERS, X_PATT, Z_PATT, FRAME, BLACKOUT };

typedef struct {
    char brd[5][5];
    int call_char[30];
    int call_val[30];
    game_state this_state;

} CHECK_PLAY;

const int DPI = 203;        //used for bitmap

int getLogo2(canvas* can, const char* str, int brightness, int contrast);
int getLogo(canvas* can, const char* str);

int getTXT_A(canvas* can, const char* str);
int getTXT_B(canvas* can, const char* str);
int getTXT_C(canvas* can, const char* str);
int getTXT_D(canvas* can, const char* str);

int getToekn(std::string* str);

int create_cell(canvas* can, char num, int c_size, bool idk);
int create_cell(canvas* can, std::string *str, int c_sizeX, int c_sizeY);
int create_play_area(canvas* can, CHECK_PLAY *game);
int get_calls(canvas* can, CHECK_PLAY* game);
int gen_cb(canvas* can, int c_size);
void gen_table(char* tbl, int x0, int y0);
void gen_calls(std::string* tbl, int num, CHECK_PLAY* game);

void debug_print(char ptr[][5]);
void debug_state(game_state gs);

game_state solve_game(CHECK_PLAY* game);

int calc_offset(int32_t m, int32_t s)
{
    return (m - s) / 2;
}


int main(int argc, char* argv[])
{
    srand((unsigned)time(NULL) * _getpid());

    CHECK_PLAY my_game;

    std::string DB_TOKEN;
    std::string FILE_PATH = std::string(SAVE_AS);

    canvas* temp;
    canvas master;
    
    getToekn(&DB_TOKEN);
    FILE_PATH += (std::string("\\") + DB_TOKEN + std::string(".bmp"));

    /* runtime debug */
    printf("FilePath: %s\n",    FILE_PATH.c_str());
    printf("Token: %s\n",       DB_TOKEN.c_str());

    /* Create Master Canvas */
    if (master.create(812, 1624) == 1)                              return 1;

    /* open logo */
    temp = new canvas;
    if (getLogo(temp, LOGO) == 1)                                   return 1;
    if (master.addSprite(temp, 
        calc_offset(master.get_x(), temp->get_x()),
        30, 0) == 1)                                                return 1;
    delete temp;

    /* open logo part 2 */
    temp = new canvas;
    if (getLogo2(temp, LIPS, 0, 0) == 1)                            return 1;
    if (master.addSprite(temp, 
        210, 190, 1) == 1)                                          return 1;
    delete temp;

    /* gen play field */
    temp = new canvas;
    if (create_play_area(temp, &my_game) == 1)                      return 1;
    if (master.addSprite(temp, 
        calc_offset(master.get_x(), temp->get_x()), 
        320, 0) == 1)                                               return 1;
    delete temp;

    /* call lable */
    temp = new canvas;
    if (getTXT_D(temp, "CALL NUMBERS") == 1)                        return 1;
    if (master.addSprite(temp,
        calc_offset(master.get_x(), temp->get_x()),
        1000, 0) == 1)                                              return 1;
    delete temp;

    /* gen calls field */
    temp = new canvas;
    if (get_calls(temp, &my_game) == 1)                             return 1;
    if (master.addSprite(temp,
        calc_offset(master.get_x(), temp->get_x()),
        1100, 0) == 1)                                              return 1;
    delete temp;

    /* TEMP */
    game_state BCD = solve_game(&my_game);
    debug_state(BCD);

    /* Save as MONO Thermal Bitmap */
    if (master.saveBMP(FILE_PATH.c_str(), DPI) == 1)                return 1;

    return 0;
}

/* AI MAGIC */
game_state solve_game(CHECK_PLAY* game)
{
    int zc = 1;
    game->brd[2][2] = 0; //free space
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 5; j++) {
            if (game->brd[j][  game->call_char[i] ] == game->call_val[i]) {
                game->brd[j][  game->call_char[i] ] = 0;
                zc++;
            }
        }
    }
    debug_print(game->brd);

    int forward_slash = 0, back_slash = 0, bottom_row = 0, top_row = 0;
    int right_row = 0, left_row = 0, four_cor = 0, black_out = 0, extra = 0;
    int any_row = 0;

    for (int i = 0; i < 5; i++)
        forward_slash += game->brd[4-i][i];

    for (int i = 0; i < 5; i++)
        back_slash += game->brd[i][i];

    for (int i = 0; i < 5; i++) {
        bottom_row += game->brd[4][i];
        top_row += game->brd[0][i];
        right_row += game->brd[i][4];
        left_row += game->brd[i][0];
    }

    for (int i = 0; i < 5; i++) {
        int g = 0, h = 0;
        for (int j = 0; j < 5; j++) {
            g += game->brd[i][j];
            h += game->brd[j][i];
        }
        if (g == 0 || h == 0) any_row++;
    }

    extra = game->brd[1][2] + game->brd[3][2] + game->brd[2][1] + game->brd[2][3];
    four_cor = game->brd[0][0] + game->brd[4][4] + game->brd[4][0] + game->brd[0][4];
    black_out = forward_slash + back_slash + bottom_row + top_row + right_row + left_row + extra;

    if (black_out == 0)                                     return game_state::BLACKOUT;
    if ((bottom_row + top_row + right_row + left_row) == 0) return game_state::FRAME;
    if ((bottom_row + top_row + forward_slash) == 0)        return game_state::Z_PATT;
    if ((forward_slash + back_slash) == 0)                  return game_state::X_PATT;
    if (four_cor == 0)                                      return game_state::FOUR_CORNERS;
    if (any_row != 0)                                       return game_state::FIVE_IN_ANY_ROW;

    return game_state::LOST;
}

int get_calls(canvas* can, CHECK_PLAY* game)
{
    bool err = 0;
    const int cell_sizeX = 140, cell_sizeY = 50;
    err |= can->create((cell_sizeX * 5), (cell_sizeY * 6), 0);

    std::string* ptr = new std::string[30];
    gen_calls(ptr, 30, game);

    for (int y = 0; y < 6; y++) {
        for (int x = 0; x < 5; x++) {
            canvas* temp = new canvas;
            err |= (bool)create_cell(temp, &ptr[x + (y * 5)], cell_sizeX, cell_sizeY);
            err |= can->addSprite(temp, (x * cell_sizeX), (y * cell_sizeY), 0);
            delete temp;
        }
    }

    delete[] ptr;
    return err;
}

int create_cell(canvas* can, std::string* str, int c_sizeX, int c_sizeY)
{
    bool err = 0;
    canvas* temp = new canvas;
    err |= can->create(c_sizeX, c_sizeY, 0);
    err |= (bool)getTXT_C(temp, str->c_str());
    err |= can->addSprite(temp,
        calc_offset(can->get_x(), temp->get_x()),
        calc_offset(can->get_y(), temp->get_y()),
        1);

    delete temp;
    return err;
}

int create_cell(canvas* can, char num, int c_size, bool idk = 0)
{
    bool err = 0;
    canvas* temp = new canvas;
    err |= can->create(c_size, c_size, 0);
    err |= can->drawBox(0, 0, c_size+1, c_size+1, 1, 1);

    if (idk) //char
    {
        err |= (bool)getTXT_C(temp, std::string(1, num).c_str());
    }
    else     //num
    {
        char tmp[3];
        snprintf(tmp, 3, "%02d", num);
        err |= (bool)getTXT_C(temp, tmp);
    }

    err |= can->addSprite(temp,
        calc_offset(can->get_x(), temp->get_x()),
        calc_offset(can->get_y(), temp->get_y()),
        1);

    delete temp;
    return err;
}

int create_play_area(canvas* can, CHECK_PLAY* game)
{
    bool err = 0;
    const int cell_size = 100;
    err |= can->create((cell_size * 5), (cell_size * 6), 0);

    char table[5 * 6];
    memset(table, 0x00, (5 * 6) * sizeof(char));
    table[0] = 'B';
    table[1] = 'I';
    table[2] = 'N';
    table[3] = 'G';
    table[4] = 'O';

    gen_table(&table[5], 5, 5);
    memcpy(&game->brd, &table[5], 5 * 5); //copy board

    for (int y = 0; y < 6; y++) {
        for (int x = 0; x < 5; x++) {

            canvas* temp = new canvas;
            char *k = &table[x + (y * 5)];

            if (x + (y * 5) < 5) {
                err |= (bool) create_cell(temp, *k, cell_size, 1);
            } else {                      
                err |= (bool) create_cell(temp, *k, cell_size);
            }

            // gen free spaces
            if (x == 2 && y == 3) {
                temp->fill(0);
                gen_cb(temp, cell_size);
            }

            err |= can->addSprite(temp, (x * cell_size), (y * cell_size), 0);
            delete temp;
        }
    }

    err |= can->drawBox(0, 0, (cell_size * 6), (cell_size * 5), 1, 1);

    return err;
}

int gen_cb(canvas* can, int c_size)
{
    int32_t K = can->get_x() * can->get_y();
    bool* ptr = can->get_pointer();

    for (int32_t i = 0; i < K; i++) 
        ptr[i] = (i % 6) ? 0 : 1; //pixle hack!

    return can->drawBox(0, 0, c_size + 1, c_size + 1, 1, 1);
}

/* This should mind the rules of 'BINGO' */
void gen_table(char* tbl, int x0 = 5, int y0 = 5)
{
    for (int y = 0; y < y0; y++) {
        for (int x = 0; x < x0; x++) {
            bool ch = 0;
            char* k = &tbl[x + (y * y0)];
            do
            {
                ch = 0;
                *k = (rand() % 15) + (15 * x) + 1;
                for (int i = 0; i < (x0*y0); i++)
                    if (k != &tbl[i])
                        ch |= (*k == tbl[i]);
            } while (ch);
        }
    }
    return; //break point
}

/* This should mind the rules of 'BINGO' too */
void gen_calls(std::string *tbl, int num, CHECK_PLAY* game)
{
    // !!! due to game check, num needs to be = to 30 !!!

    char bingo[] = { 'B', 'I', 'N', 'G', 'O' };
    for (int i = 0; i < 30; i++) {
        char tmp[3];
        bool ch = 0;
        game->call_char[i] = rand() % 5;
        do {
            ch = 0;
            game->call_val[i] = (rand() % 15) + (15 * game->call_char[i]) + 1;
            for (int j = 0; j < 30; j++)
                if (i != j)
                    ch |= (game->call_val[i] == game->call_val[j]);
        } while (ch);
        snprintf(tmp, 3, "%02d", game->call_val[i]);
        tbl[i] = std::string(1, bingo[game->call_char[i]]) + std::string(tmp);
    }
    return; //break point
}

int getToekn(std::string* str)
{
    str->clear();
    for (int i = 0; i < 10; i++) {
        int h = rand() % 10;
        *str += std::to_string(h);
    }

    return 0;
}

int getTXT_A(canvas* can, const char* str)
{
    bool err = 0;
    font t;
    err |= t.create(FONT_ARIAL, 8, DPI);
    err |= t.writeCanvas(can, str);
    return err;
}

int getTXT_B(canvas* can, const char* str)
{
    bool err = 0;
    font t;
    err |= t.create(FONT_ARIAL, 10, DPI);
    err |= t.writeCanvas(can, str);
    return err;
}

int getTXT_C(canvas* can, const char* str)
{
    bool err = 0;
    font t;
    err |= t.create(FONT_ARIAL, 12, DPI);
    err |= t.writeCanvas(can, str);
    return err;
}

int getTXT_D(canvas* can, const char* str)
{
    bool err = 0;
    font t;
    err |= t.create(FONT_ARIAL, 16, DPI);
    err |= t.writeCanvas(can, str);
    return err;
}

int getLogo(canvas* can, const char* str)
{
    return can->import_24bit(str, DITHER::Threshold);
}

int getLogo2(canvas* can, const char *str, int brightness, int contrast)
{
    return can->import_24bit(str, DITHER::Stucki, brightness, contrast);
}

void debug_print(char ptr[][5])
{
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++)
            printf("%02d ", ptr[i][j]);
        printf("\n");
    }
}

void debug_state(game_state gs)
{
    switch (gs)
    {
        case game_state::BLACKOUT:         printf("WIN - BLACKOUT\n");     break;
        case game_state::FIVE_IN_ANY_ROW:  printf("WIN - 5 IN ANY ROW\n"); break;
        case game_state::FOUR_CORNERS:     printf("WIN - 4 CORNERS\n");    break;
        case game_state::FRAME:            printf("WIN - FRAME\n");        break;
        case game_state::X_PATT:           printf("WIN - X PATERN\n");     break;
        case game_state::Z_PATT:           printf("WIN - Z PATTERN\n");    break;
        default:                           printf("LOST\n");               break;
    };
    return; //break point
}