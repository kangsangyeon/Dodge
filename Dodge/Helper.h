#pragma once
#include <stdbool.h>
#include <wchar.h>

char* WcharStringToCharString(wchar_t* _source);

bool LoadTextFileAsMatrix2D(wchar_t* _filePath, wchar_t*** _outData, int* _outWidth, int* _outHeight);
