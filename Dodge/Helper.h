#pragma once
#include <stdbool.h>
#include <wchar.h>

char* WcharStringToCharString(wchar_t* _source);

bool LoadTextFileAsMatrix2D(wchar_t* _filePath, wchar_t*** _outData, int* _outWidth, int* _outHeight);

bool LoadTextFileAsBoolMatrix2D(wchar_t* _filePath, bool*** _outData, int* _outWidth, int* _outHeight);

double FClamp(double _value, double _min, double _max);

int Clamp(int _value, int _min, int _max);
