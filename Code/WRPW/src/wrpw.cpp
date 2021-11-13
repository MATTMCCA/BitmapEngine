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