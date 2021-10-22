#include "Sprite.h"

#include <stdio.h>
#include <stdlib.h>

#include "PointerStack.h"

const int MAXIMUM_TEXT_LINE = 1000;

Sprite* loadSpriteFromText(wchar_t* _filePath)
{
	Sprite* _outSprite = (Sprite*)calloc(1, sizeof(Sprite));

	PointerStack* _stack = PointerStack_Create(100);
	int _spriteWidth = 0;

	FILE* _fileStream;
	fopen_s(&_fileStream, "test.txt", "r,ccs=UTF-8");

	if (_fileStream == NULL)
	{
		// 파일을 여는 데 실패한 경우, 함수의 실행을 바로 종료합니다.
		return NULL;
	}

	// 파일을 읽고 각 라인을 stack에 담습니다.
	for (int _i = 0; ; ++_i)
	{
		wchar_t* _string = (wchar_t*)malloc((MAXIMUM_TEXT_LINE) * sizeof(wchar_t));
		fgetws(_string, MAXIMUM_TEXT_LINE, _fileStream);

		// 파일의 끝에 도달했을 때 읽은 줄은 무시합니다.
		if (feof(_fileStream) != 0)
			break;

		int _strLength = wcslen(_string);

		// 텍스트 파일의 끝이 아니라면 읽어들인 문자열의 끝에 줄바꿈 문자를 읽어들였을 것입니다.
		// 렌더링할 때 문제가 생기지 않도록 줄바꿈 문자를 제거합니다.
		if (_string[_strLength - 1] == L'\n')
		{
			_string[_strLength - 1] = '\0';
			_strLength -= 1;
		}

		// spriteWidth를 구하기 위해 파일 내 모든 라인 중 가장 긴 라인의 텍스트 개수를 추적합니다.
		if (_spriteWidth < _strLength)
			_spriteWidth = _strLength;

		PointerStack_Push(_stack, _string);
	}

	fclose(_fileStream);
	_fileStream = NULL;

	// stack에 차례로 담긴 각 라인을 빼내어 Sprite에 사용될 문자열(이중 배열)을 생성합니다.
	const int _spriteHeight = PointerStack_GetSize(_stack);
	const int _spriteLineByteSize = (_spriteWidth + 1) * sizeof(wchar_t);

	wchar_t** _dataArr = (wchar_t**)calloc(_spriteHeight, sizeof(wchar_t*));

	for (int _y = 0; _y < _spriteHeight; ++_y)
	{
		const wchar_t* _popped = (wchar_t*)PointerStack_Pop(_stack);
		const int _poppedByteSize = wcslen(_popped) * sizeof(wchar_t);

		wchar_t* _line = (wchar_t*)calloc(_spriteWidth + 1, sizeof(wchar_t));
		memcpy_s(_line, _spriteLineByteSize, _popped, _poppedByteSize);

		_dataArr[_y] = _line;

		free(_popped);
	}

	PointerStack_Release(_stack);

	_outSprite->width = _spriteWidth;
	_outSprite->height = _spriteHeight;
	_outSprite->dataArr = _dataArr;

	return _outSprite;
}

void releaseSprite(Sprite* _sprite)
{
	if (_sprite == NULL)
		return;

	if (_sprite->dataArr != NULL)
	{
		for (int i = 0; i < _sprite->height; ++i)
		{
			if (_sprite->dataArr[i] != NULL)
				free(_sprite->dataArr[i]);
		}

		free(_sprite->dataArr);
	}

	free(_sprite);
}
