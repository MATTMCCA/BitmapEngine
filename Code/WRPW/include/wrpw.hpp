/*
====================================================================================== =

	File: wrpw.hpp

	Notes : Minimal raw print wrapper for windows.

	MJM - 11/13/2021

====================================================================================== =
*/

#pragma once

#include <iostream>

/* windows junk */
#include <atlstr.h>
#include <tchar.h>
#include <Windows.h>

//https://docs.microsoft.com/en-us/windows/win32/printdocs/sending-data-directly-to-a-printer
BOOL RawDataToPrinter(LPTSTR szPrinterName, LPTSTR docName, LPBYTE lpData, DWORD dwCount);

/* wrapper function */
bool raw_print(const char* printer, uint8_t* ptr, uint32_t size, const char* doc_name = "RAW_DOC");

//https://stackoverflow.com/questions/34279427/how-to-get-printer-manufacturer-and-model-from-win32-api
void list_printers(void);