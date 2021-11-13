#pragma once

/* windows junk */
#include <atlstr.h>
#include <tchar.h>
#include <Windows.h>

//https://docs.microsoft.com/en-us/windows/win32/printdocs/sending-data-directly-to-a-printer
BOOL RawDataToPrinter(LPTSTR szPrinterName, LPTSTR docName, LPBYTE lpData, DWORD dwCount);

/* wrapper function */
bool raw_print(const char* printer, uint8_t* ptr, uint32_t size, const char* doc_name = "RAW_DOC");