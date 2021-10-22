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
		// ������ ���� �� ������ ���, �Լ��� ������ �ٷ� �����մϴ�.
		return NULL;
	}

	// ������ �а� �� ������ stack�� ����ϴ�.
	for (int _i = 0; ; ++_i)
	{
		wchar_t* _string = (wchar_t*)malloc((MAXIMUM_TEXT_LINE) * sizeof(wchar_t));
		fgetws(_string, MAXIMUM_TEXT_LINE, _fileStream);

		// ������ ���� �������� �� ���� ���� �����մϴ�.
		if (feof(_fileStream) != 0)
			break;

		int _strLength = wcslen(_string);

		// �ؽ�Ʈ ������ ���� �ƴ϶�� �о���� ���ڿ��� ���� �ٹٲ� ���ڸ� �о�鿴�� ���Դϴ�.
		// �������� �� ������ ������ �ʵ��� �ٹٲ� ���ڸ� �����մϴ�.
		if (_string[_strLength - 1] == L'\n')
		{
			_string[_strLength - 1] = '\0';
			_strLength -= 1;
		}

		// spriteWidth�� ���ϱ� ���� ���� �� ��� ���� �� ���� �� ������ �ؽ�Ʈ ������ �����մϴ�.
		if (_spriteWidth < _strLength)
			_spriteWidth = _strLength;

		PointerStack_Push(_stack, _string);
	}

	fclose(_fileStream);
	_fileStream = NULL;

	// stack�� ���ʷ� ��� �� ������ ������ Sprite�� ���� ���ڿ�(���� �迭)�� �����մϴ�.
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
