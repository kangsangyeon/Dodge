#include "Helper.h"

#include <stdlib.h>

char* WcharStringToCharString(wchar_t* _source)
{
	const int _sourceLength = wcslen(_source);
	const int _outStringByteSize = sizeof(char) * _sourceLength + 1;

	size_t _numOfCharConverted;

	char* _outString = (char*)malloc(_outStringByteSize);
	wcstombs_s(&_numOfCharConverted, _outString, _outStringByteSize, _source, _outStringByteSize - 1);

	return _outString;
}
