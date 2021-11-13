/*
====================================================================================== =

    File: wrpw.cpp

    Notes : Minimal raw print wrapper for windows.

    MJM - 11/13/2021

====================================================================================== =
*/

#include "wrpw.hpp"

/*******************************************************************/
// https://docs.microsoft.com/en-us/windows/win32/printdocs/sending-data-directly-to-a-printer
// https://download.microsoft.com/download/platformsdk/utility/95/win98/en-us/rawprn.exe
// 
// RawDataToPrinter - sends binary data directly to a printer 
//  
// szPrinterName: NULL-terminated string specifying printer name 
// lpData:        Pointer to raw data bytes 
// dwCount        Length of lpData in bytes 
//  
// Returns: TRUE for success, FALSE for failure. 
//  
BOOL RawDataToPrinter(LPTSTR szPrinterName, LPTSTR docName, LPBYTE lpData, DWORD dwCount)
{
    BOOL       bStatus = FALSE;
    HANDLE     hPrinter = NULL;
    DOC_INFO_1 DocInfo;
    DWORD      dwJob = 0L;
    DWORD      dwBytesWritten = 0L;

    // Open a handle to the printer. 
    bStatus = OpenPrinter(szPrinterName, &hPrinter, NULL);
    if (bStatus) {
        // Fill in the structure with info about this "document." 
        DocInfo.pDocName = docName;
        DocInfo.pOutputFile = NULL;
        DocInfo.pDatatype = (LPTSTR)_T("RAW");

        // Inform the spooler the document is beginning. 
        dwJob = StartDocPrinter(hPrinter, 1, (LPBYTE)&DocInfo);
        if (dwJob > 0) {
            // Start a page. 
            bStatus = StartPagePrinter(hPrinter);
            if (bStatus) {
                // Send the data to the printer. 
                bStatus = WritePrinter(hPrinter, lpData, dwCount, &dwBytesWritten);
                EndPagePrinter(hPrinter);
            }
            // Inform the spooler that the document is ending. 
            EndDocPrinter(hPrinter);
        }
        // Close the printer handle. 
        ClosePrinter(hPrinter);
    }
    // Check to see if correct number of bytes were written. 
    if (!bStatus || (dwBytesWritten != dwCount)) {
        bStatus = FALSE;
    }
    else {
        bStatus = TRUE;
    }
    return bStatus;
}

bool raw_print(const char* printer, uint8_t* ptr, uint32_t size, const char* doc_name)
{
    return !RawDataToPrinter(
        (LPTSTR)CString(printer).AllocSysString(),
        (LPTSTR)CString(doc_name).AllocSysString(),
        ptr, size);
}

/* shamelessly CTRL+C && CTRL+V */
// https://stackoverflow.com/questions/34279427/how-to-get-printer-manufacturer-and-model-from-win32-api

void list_printers(void)
{
    int level = 2;
    DWORD flags = PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS;
    PRINTER_INFO_2* printerInfo;
    DWORD memsize, printer_count;

    EnumPrinters(flags, NULL, level, NULL, 0, &memsize, &printer_count);
    if (memsize < 1) 
        return;

    BYTE* bytes = new BYTE[memsize];
    if (EnumPrinters(flags, NULL, level, bytes, memsize, &memsize, &printer_count)) {
        printerInfo = (PRINTER_INFO_2*)bytes;
        for (UINT i = 0; i < printer_count; i++) {
            std::wcout << "printer: "                  << printerInfo->pPrinterName  << "\n";
            std::wcout << "printerInfo->Status: "      << printerInfo->Status        << "\n";
            std::wcout << "printerInfo->pDriverName: " << printerInfo->pDriverName   << "\n";
            std::wcout << "printerInfo->pShareName: "  << printerInfo->pShareName    << "\n";
            std::wcout << "printerInfo->pPortName: "   << printerInfo->pPortName     << "\n\n";
            printerInfo++;
        }
    }
    delete[] bytes;
}