#include "Helper.h"

#include <stdio.h>
#include <stdlib.h>

#include "PointerStack.h"

const int MAXIMUM_TEXT_LINE = 1000;

char* WcharStringToCharString(wchar_t* _source)
{
	const int _sourceLength = wcslen(_source);
	const int _outStringByteSize = sizeof(char) * _sourceLength + 1;

	size_t _numOfCharConverted;

	char* _outString = (char*)malloc(_outStringByteSize);
	wcstombs_s(&_numOfCharConverted, _outString, _outStringByteSize, _source, _outStringByteSize - 1);

	return _outString;
}

bool LoadTextFileAsMatrix2D(wchar_t* _filePath, wchar_t*** _outData, int* _outWidth, int* _outHeight)
{
	const char* _filePathStr = WcharStringToCharString(_filePath);

	FILE* _fileStream;
	fopen_s(&_fileStream, _filePathStr, "r,ccs=UTF-8");

	free(_filePathStr);

	if (_fileStream == NULL)
	{
		// 파일을 여는 데 실패한 경우, 함수의 실행을 바로 종료합니다.
		return false;
	}

	PointerStack* _stack = PointerStack_Create(100);
	int _fileWidth = 0;

	// 파일을 읽고 각 라인을 stack에 담습니다.
	for (int _i = 0; ; ++_i)
	{
		// 파일의 끝에 도달했을 때 파일 읽기를 종료합니다.
		if (feof(_fileStream) != 0)
			break;

		wchar_t* _string = (wchar_t*)malloc((MAXIMUM_TEXT_LINE) * sizeof(wchar_t));
		fgetws(_string, MAXIMUM_TEXT_LINE, _fileStream);

		int _strLength = wcslen(_string);

		// 텍스트 파일의 끝이 아니라면 읽어들인 문자열의 끝에 줄바꿈 문자를 읽어들였을 것입니다.
		// 렌더링할 때 문제가 생기지 않도록 줄바꿈 문자를 제거합니다.
		if (_string[_strLength - 1] == L'\n')
		{
			_string[_strLength - 1] = '\0';
			_strLength -= 1;
		}

		// spriteWidth를 구하기 위해 파일 내 모든 라인 중 가장 긴 라인의 텍스트 개수를 추적합니다.
		if (_fileWidth < _strLength)
			_fileWidth = _strLength;

		PointerStack_Push(_stack, _string);
	}

	fclose(_fileStream);
	_fileStream = NULL;

	// stack에 차례로 담긴 각 라인을 빼내어 Sprite에 사용될 문자열(이중 배열)을 생성합니다.
	const int _fileHeight = PointerStack_GetSize(_stack);
	const int _fileLineByteSize = (_fileWidth + 1) * sizeof(wchar_t);

	wchar_t** _dataArr = (wchar_t**)calloc(_fileHeight, sizeof(wchar_t*));

	for (int _y = 0; _y < _fileHeight; ++_y)
	{
		const wchar_t* _popped = (wchar_t*)PointerStack_Pop(_stack);
		const int _poppedByteSize = wcslen(_popped) * sizeof(wchar_t);

		wchar_t* _line = (wchar_t*)calloc(_fileWidth + 1, sizeof(wchar_t));
		memcpy_s(_line, _fileLineByteSize, _popped, _poppedByteSize);

		_dataArr[_y] = _line;

		free(_popped);
	}

	PointerStack_Release(_stack);

	// 값을 반환합니다.
	if (_outData != NULL)
		*_outData = _dataArr;

	if (_outWidth != NULL)
		*_outWidth = _fileWidth;

	if (_outHeight != NULL)
		*_outHeight = _fileHeight;

	return true;
}

double FClamp(double _value, double _min, double _max)
{
	if (_value < _min)
		return _min;

	if (_value > _max)
		return _max;

	return _value;
}

int Clamp(int _value, int _min, int _max)
{
	if (_value < _min)
		return _min;

	if (_value > _max)
		return _max;

	return _value;
}
