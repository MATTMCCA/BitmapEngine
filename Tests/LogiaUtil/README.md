


# LogiaUtil

Minimal utility for printing image on a [Logia 300DPI Thermal printer](https://www.amazon.com/Logia-High-Speed-Commercial-Compatible-Stamps-com/dp/B08G2YL161).

This was build as an example project.

# HelpFile
```
λ LogiaUtil.exe

 _                  _          ______   _____   _____  ______  ______  _
(_)                (_)        (_____ \ (_____) (_____)(______)(_____ \| |
 _       ___   ____ _ _____    _____) )_  __ _ _  __ _ _     _ _____) ) |
| |     / _ \ / _  | (____ |  (_____ (| |/ /| | |/ /| | |   | |  ____/| |
| |____| |_| ( (_| | / ___ |   _____) )   /_| |   /_| | |__/ /| |     | |
|_______)___/ \___ |_\_____|  (______/ \_____/ \_____/|_____/ |_|     |_|
             (_____|       Logia [ZPL] PNG ImagePrint Utlity [MJM - 2021]
                                (Not Affiliated With Logiaprinters)
-------------------------------------------------------------------------
Options:
  -p ......... Printer Name         (Print)
  -z ......... ZPL file Name        (Save)
  -m ......... BMP file Name        (save)
  -a ......... MCB file Name        (save) (Requires -j)
     -j ...... MCB HeadSize         [DOTS] (1280 or 832)
  -e ......... PBM file Name        (save)
  -f ......... XBM file Name        (save) (Requires -r)
     -r ...... XBM Struct Name
  -n ......... PNG file Name        (Input File)
  -x ......... List Printers        (Debug Option)
  -w ......... Print Width          [DOTS] (dpi x inch)
  -l ......... Label Length         [DOTS] (dpi x inch)
  -s ......... Print Speed          [1 - 8]
  -d ......... Darkness             [1 - 8]
  -q ......... Quantity             [1 - nth]
  -g ......... Continuous Media     [Default: Auto]
  -t ......... Dither Type          [0 - 9]
         0 --> Threshold
         1 --> FloydSteinberg
         2 --> Stucki
         3 --> Jarvis
         4 --> Atkinson
         5 --> Bayer_2x2
         6 --> Bayer_4x4
         7 --> Bayer_8x8
         8 --> Bayer_16x16
         9 --> Cluster
  -b ......... Brightness       [-255 - 255]
  -c ......... Contrast         [-255 - 255]
  -i ......... Invert
  -v ......... Verbose
-------------------------------------------------------------------------

LogiaUtil.exe -z "moo.zpl" -w 600 -l 600 -s 8 -d 8 -q 1 -t 1 -n "Lena.png" -m "moo.bmp" -p "Logia Printer"

   Print to   "Logia Printer"
   Save as    moo.bmp & moo.zpl
   Img      = Lena.png
   Witdh    = 600 dots (600/300 = 2")
   Length   = 600 dots (600/300 = 2")
   Quantity = 1
   Speed    = 8
   Darkness = 8
   Dither   = FloydSteinberg
-------------------------------------------------------------------------
Other Example:
[Simple File Conversion]
   LogiaUtil.exe -w 600 -l 600  -n "Lena.png" -m "outputImg.bmp" -t 1
   LogiaUtil.exe -w 600 -l 600  -n "Lena.png" -j 1280 -a "outputImg.bin" -t 1
   LogiaUtil.exe -w 600 -l 600  -n "Lena.png" -e "outputImg.pbm" -t 1
   LogiaUtil.exe -w 600 -l 600  -n "Lena.png" -r "catstruct" -f "outputImg.xbm" -t 1

[List Installed Printers]
   LogiaUtil.exe -x
```


# Stupid Printer Tricks 

[How to Assign a USB Printer to a LPT Port](https://supportcommunity.zebra.com/s/article/How-to-Assign-USB-Printers-to-LPT-Ports?language=en_US)

[Print RAW File From CMD](https://supportcommunity.zebra.com/s/article/Print-RAW-File-From-CMD?language=en_US)

[Capture Print Driver Output](https://supportcommunity.zebra.com/s/article/Capture-Printer-Output-to-a-File-using-FILE-port?language=en_US)

[ZPL Viewer](http://labelary.com/viewer.html)

[Send Data Directly to a GDI Printer](https://docs.microsoft.com/en-us/windows/win32/printdocs/sending-data-directly-to-a-printer)

#### Batch file for printing RAW over VCP/Serial
```
REM Raw Print Batch
MODE COM21 BAUD=115200 PARITY=n DATA=8
COPY yourfile.txt /B \\.\COM21 /B
PAUSE
```

#### Batch file for printing RAW to LPT
```
REM Raw Print Batch
REM See LPT Link above
COPY /B yourfile.txt LPT2
PAUSE
```


