/*
    ***************************************************************************************
    MJM -2021-

    LogiaUtil

    This is a Test utility for printing png's to a Logia Thermal Printer via commandline

    ---------------------------------------------------------------------------------------

    =======================================================================================

        Notes: 
            Logia Thermal Printer Test (I'm pore, its a cheap 300DPI printer lol)

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

#include <string>
#include <cstdio>
#include <cstdint>
#include <cctype>

#include "canvas.hpp"       // Canvas Header
#include "zpl.hpp"          // ZPL Format header
#include "getopt.h"         // *NIX header for windows {kinda?}

/* Added for !PAZAZ! */
const char* logo = R"(
 _                  _          ______   _____   _____  ______  ______  _ 
(_)                (_)        (_____ \ (_____) (_____)(______)(_____ \| |
 _       ___   ____ _ _____    _____) )_  __ _ _  __ _ _     _ _____) ) |
| |     / _ \ / _  | (____ |  (_____ (| |/ /| | |/ /| | |   | |  ____/| |
| |____| |_| ( (_| | / ___ |   _____) )   /_| |   /_| | |__/ /| |     | |
|_______)___/ \___ |_\_____|  (______/ \_____/ \_____/|_____/ |_|     |_|
             (_____|       Logia [ZPL] PNG ImagePrint Utlity [MJM - 2021]
                                (Not Affiliated With Logiaprinters)                                            
)";

/* default printer parms */
PARM DEFAULT = {
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

/*
int32_t calc_offset(int32_t m, int32_t s) { return (m - s) / 2; }
*/

void printUsage(const char *c)
{
    printf("%s", logo);
    printf("-------------------------------------------------------------------------\n");
    printf("Options:\n");
    printf("  -p ......... Printer Name         (Print)\n");
    printf("  -z ......... ZPL file Name        (Save)\n");
    printf("  -m ......... BMP file Name        (save)\n");
    printf("  -a ......... MCB file Name        (save) (Requires -j)\n");
    printf("     -j ...... MCB HeadSize         [DOTS] (1280 or 832)\n");
    printf("  -e ......... PBM file Name        (save)\n");
    printf("  -f ......... XBM file Name        (save) (Requires -r)\n");
    printf("     -r ...... XBM Struct Name \n");
    printf("  -n ......... PNG file Name        (Input File)\n");
    printf("  -x ......... List Printers        (Debug Option)\n");
    printf("  -w ......... Print Width          [DOTS] (dpi x inch)\n");
    printf("  -l ......... Label Length         [DOTS] (dpi x inch)\n");
    printf("  -s ......... Print Speed          [1 - 8]\n");
    printf("  -d ......... Darkness             [1 - 8]\n");
    printf("  -q ......... Quantity             [1 - nth]\n");
    printf("  -t ......... Dither Type          [0 - 9]\n");
    printf("         0 --> Threshold\n");
    printf("         1 --> FloydSteinberg\n");
    printf("         2 --> Stucki\n");
    printf("         3 --> Jarvis\n");
    printf("         4 --> Atkinson\n");
    printf("         5 --> Bayer_2x2\n");
    printf("         6 --> Bayer_4x4\n");
    printf("         7 --> Bayer_8x8\n");
    printf("         8 --> Bayer_16x16\n");
    printf("         9 --> Cluster\n");
    printf("  -b ......... Brightness       [-255 - 255]\n");
    printf("  -c ......... Contrast         [-255 - 255]\n");
    printf("  -i ......... Invert\n");
    printf("  -v ......... Verbose\n");
    printf("-------------------------------------------------------------------------\n\n");
    printf("%s -z \"moo.zpl\" -w 600 -l 600 -s 8 -d 8 -q 1 -t 1 -n \"Lena.png\" -m \"moo.bmp\" -p \"Logia Printer\"\n\n", c);
    printf("   Print to   \"Logia Printer\"\n");
    printf("   Save as    moo.bmp & moo.zpl\n");
    printf("   Img      = Lena.png\n");
    printf("   Witdh    = 600 dots (600/300 = 2\")\n");
    printf("   Length   = 600 dots (600/300 = 2\")\n");
    printf("   Quantity = 1\n");
    printf("   Speed    = 8\n");
    printf("   Darkness = 8\n");
    printf("   Dither   = FloydSteinberg\n");
    printf("-------------------------------------------------------------------------\n");
}

int main(int argc, char* argv[])
{
    int opt;
    int optionSelected;
    std::size_t inputValLen;

    std::string PRINTERNAME;
    std::string INPUT;
    std::string ZPLOUTPUT;
    std::string BMPOUTPUT;
    std::string MCBOUTPUT;
    std::string PBMOUTPUT;
    std::string XBMOUTPUT;
    std::string XBMSTRUCT;

    int32_t CONTRAST     = 0;
    int32_t BRIGHT       = 0;
    int32_t DITHERTYPE   = 0;
    int32_t HEADSIZE     = 832;
    bool invert          = 0;
    bool verbose         = 0;

    if (argc <= 1) {
        printUsage(argv[0]);
        return -1;
    }

    try {
        while ((opt = getopt(argc, argv, "p:z:w:l:s:d:q:n:t:b:c:m:a:e:f:j:r:ixv")) != -1) {
            switch (opt)
            {
            case 'p':
                PRINTERNAME = optarg;
                break;
            case 'z':
                ZPLOUTPUT = optarg;
                break;
            case 'm':
                BMPOUTPUT = optarg;
                break;
            case 'a':
                MCBOUTPUT = optarg;
                break;
            case 'e':
                PBMOUTPUT = optarg;
                break;
            case 'f':
                XBMOUTPUT = optarg;
                break;
            case 'r':
                XBMSTRUCT = optarg;
                break;
            case 'w':
                optionSelected = std::stoi(optarg, &inputValLen);
                DEFAULT.print_width.label_width = optionSelected;
                break;
            case 'l':
                optionSelected = std::stoi(optarg, &inputValLen);
                DEFAULT.label_length.y_axis_pos = optionSelected;
                break;
            case 's':
                optionSelected = std::stoi(optarg, &inputValLen);
                DEFAULT.print_rate.print_speed = optionSelected;
                break;
            case 'd':
                optionSelected = std::stoi(optarg, &inputValLen);
                DEFAULT.darkness.darkness_setting = optionSelected;
                break;
            case 'q':
                optionSelected = std::stoi(optarg, &inputValLen);
                DEFAULT.print_quantity.q = optionSelected;
                break;
            case 'n':
                INPUT = optarg;
                break;
            case 'j':
                optionSelected = std::stoi(optarg, &inputValLen);
                HEADSIZE = optionSelected;
                break;
            case 't':
                optionSelected = std::stoi(optarg, &inputValLen);
                DITHERTYPE = optionSelected;
                break;
            case 'b':
                optionSelected = std::stoi(optarg, &inputValLen);
                BRIGHT = optionSelected;
                break;
            case 'c':
                optionSelected = std::stoi(optarg, &inputValLen);
                CONTRAST = optionSelected;
                break;
            case 'i':
                invert = 1;
                break;
            case 'v':
                verbose = 1;
                printf("-v dose nothing....\n");
                break;
            case 'x':
                list_printers();
                return 0;
            default:
                printUsage(argv[0]);
                return -1;
            }
        }
    }
    catch (std::exception&) {
        printf("Not a Valid Argument\n");
    }

    bool err = 0;
    uint32_t x0, y0;
    canvas master;
    x0 = DEFAULT.print_width.label_width;
    y0 = DEFAULT.label_length.y_axis_pos;
    err |= master.create(x0, y0);
    if (!INPUT.empty()) {
        canvas *tmp = new canvas;
        err |= tmp->import_png(INPUT.c_str(), (DITHER)DITHERTYPE, BRIGHT, CONTRAST);
        err |= tmp->invert(invert);
        err |= master.addSprite(tmp, 0, 0, 0);
        delete tmp;
    }

    if (!BMPOUTPUT.empty())
        err |= master.saveBMP(BMPOUTPUT.c_str(), 72);

    if (!MCBOUTPUT.empty())
        err |= master.saveMCB(MCBOUTPUT.c_str(), HEADSIZE);

    if (!PBMOUTPUT.empty())
        err |= master.savePBM(PBMOUTPUT.c_str());

    if ((!XBMOUTPUT.empty()) && (!XBMSTRUCT.empty()))
        err |= master.saveXBM(XBMOUTPUT.c_str(), XBMSTRUCT.c_str());

    zpl myJob(DEFAULT);
    err |= myJob.add_graphic(master.get_pointer(), master.get_x(), master.get_y());
    err |= myJob.generate_format();

    if (!ZPLOUTPUT.empty())
        err |= myJob.save_format(ZPLOUTPUT.c_str());

    if (!PRINTERNAME.empty())
        err |= myJob.print_format(PRINTERNAME.c_str());

    if (err)  printf("[WARNING] This Program Ran with Errors\n");
    else      printf("[OK] This Program Ran without Errors\n");

    return 0;
}