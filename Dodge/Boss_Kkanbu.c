#include "Boss_Kkanbu.h"

#include <stdlib.h>

#include "DodgeGameInstance.h"
#include "GameInstance.h"

Boss_Kkanbu* Boss_Kkanbu_Create(DodgeGameInstance* _dodgeGameInstance)
{
	Boss_Kkanbu* _outBoss = (Boss_Kkanbu*)malloc(sizeof(Boss_Kkanbu));

	// boss object
	_outBoss->worldObject = WorldObject_CreateWithSpriteMask(L"Sprites/boss_dogemusk/boss_dogemusk.txt", L"Sprites/boss_dogemusk/boss_dogemusk.txt.txt",
	                                                         Vector2D_Center, Vector2D_Zero);

	_outBoss->bossStartTime = GameInstance_GetGameTime(_dodgeGameInstance->gameInstance);

	// state
	_outBoss->currentState = EBKS_START;

	// patterns
	_outBoss->currentPatternType = EBKPT_NONE;
	_outBoss->previousPatternType = EBKPT_NONE;
	_outBoss->patternStartTime = 0;
	_outBoss->patternEndTime = 0;

	return _outBoss;
}

void Boss_Kkanbu_Release(Boss_Kkanbu* _boss)
{
	if (_boss == NULL)
		return;

	if (_boss->worldObject != NULL)
		WorldObject_Release(_boss->worldObject);

	free(_boss);
}

void Boss_Kkanbu_DrawTick(DodgeGameInstance* _dodgeGameInstance, Boss_Kkanbu* _boss)
{
	if (_dodgeGameInstance == NULL || _boss == NULL)
		return;

	const Screen* _screen = _dodgeGameInstance->gameInstance->screen;

	if (_boss->worldObject != NULL && _boss->visibleWorldObject == true)
		Screen_PrintWorldObject(_screen, _boss->worldObject);
}

void Boss_Kkanbu_Tick(DodgeGameInstance* _dodgeGameInstance, Boss_Kkanbu* _boss, double _deltaTime)
{
	const bool _startNewPattern = _Boss_Kkanbu_ShouldStartNewPattern(_dodgeGameInstance, _boss);

	if (_startNewPattern == true)
	{
		// 패턴을 재생할 때가 되었을 때,
		// 패턴 관련 변수의 값들을 설정하고 패턴을 시작합니다.
		_Boss_Kkanbu_PrePattern(_dodgeGameInstance, _boss);
	}

	bool _patternEnd = false;

	switch (_boss->currentState)
	{
		// 현재 패턴을 재생합니다.
		// 만약 재생중인 패턴이 없다면 어떠한 동작도 하지 않습니다.
	case EBKS_PATTERN_PLAYING:
		_patternEnd = _Boss_Kkanbu_Pattern1Tick(_dodgeGameInstance, _boss, _deltaTime, _startNewPattern);
		break;
	}

	if (_patternEnd == true)
	{
		// 패턴이 끝난 직후라면, PostPattern을 호출합니다.
		_Boss_Kkanbu_PostPattern(_dodgeGameInstance, _boss);
	}
}

bool _Boss_Kkanbu_ShouldStartNewPattern(DodgeGameInstance* _dodgeGameInstance, Boss_Kkanbu* _boss)
{
	if (_dodgeGameInstance == NULL || _boss == NULL)
		return false;

#define PATTERN_DELAY 1.f

	const double _gameTime = GameInstance_GetGameTime(_dodgeGameInstance->gameInstance);

	switch (_boss->currentState)
	{
	case EBKS_START:
		// 보스가 시작되었을 때에는 무조건 새로운 패턴을 재생합니다.
		return true;

	case EBKS_PATTERN_DELAYED:
		{
			// 패턴을 재생하고 난 뒤 일정 시간이 흐른 뒤에 새로운 패턴을 재생합니다.
			const double _elapsedTimeAfterPatternEnd = _gameTime - _boss->patternEndTime;
			const bool _shouldStartNewPattern = _elapsedTimeAfterPatternEnd >= PATTERN_DELAY;

			return _shouldStartNewPattern;
		}
	case EBKS_PATTERN_PLAYING:
		// 이미 패턴을 재생하고 있을 때에는 새로운 패턴을 재생하지 않습니다.
		return false;
	}

	return false;
}

void _Boss_Kkanbu_PrePattern(DodgeGameInstance* _dodgeGameInstance, Boss_Kkanbu* _boss)
{
	if (_dodgeGameInstance == NULL || _boss == NULL)
		return;

#define PATTERN_COUNT 1

	const double _gameTime = GameInstance_GetGameTime(_dodgeGameInstance->gameInstance);

	// 패턴을 시작하기 앞서 변수들을 초기화합니다.
	_boss->patternStartTime = _gameTime;
	_boss->currentState = EBKS_PATTERN_PLAYING;

	// 이번에 재생할 패턴의 유형을 선택합니다.
	// 패턴의 개수가 2개 이상일 때,
	// 가장 마지막으로 재생했던 패턴의 유형은 다시 선택하지 않습니다.
	do { _boss->currentPatternType = (EBoss_Kkanbu_PatternType)(rand() % PATTERN_COUNT + 1); }
	while (PATTERN_COUNT >= 2 && _boss->currentPatternType == _boss->previousPatternType);
}

void _Boss_Kkanbu_PostPattern(DodgeGameInstance* _dodgeGameInstance, Boss_Kkanbu* _boss)
{
	if (_dodgeGameInstance == NULL || _boss == NULL)
		return;

	const double _gameTime = GameInstance_GetGameTime(_dodgeGameInstance->gameInstance);

	// 패턴이 종료된 이후 변수들을 초기화합니다.

	_boss->patternEndTime = _gameTime;

	_boss->previousPatternType = _boss->currentPatternType;
	_boss->currentState = EBKS_PATTERN_DELAYED;
	_boss->currentPatternType = EBKS_NONE;
}

bool _Boss_Kkanbu_Pattern1Tick(DodgeGameInstance* _dodgeGameInstance, Boss_Kkanbu* _boss, double _deltaTime, bool _patternStart)
{
#define BOSS_KKANBU_PATTERN1_BEHAVIOR1_DURATION 1.f
#define BOSS_KKANBU_PATTERN1_BEHAVIOR2_DURATION 1.f
#define BOSS_KKANBU_PATTERN1_BEHAVIOR2_LOOP_COUNT 4

	static int _behaviorIndex = -1;

	if (_patternStart == true)
	{
		_behaviorIndex = -1;
	}

	const double _patternElapsedTime = GameInstance_GetGameTime(_dodgeGameInstance->gameInstance) - _boss->patternStartTime;

	const float _behaviorDuration = _

	// 이번 pattern을 재생하면서 behavior를 몇 번 반복적으로 재생했는지 얻습니다.
	// 그 behavior의 경과시간도 얻습니다.
	const int _behaviorPlayedCount = (int)(_patternElapsedTime / _behaviorDuration);
	const double _behaviorElapsedTime = _patternElapsedTime - _behaviorDuration * _behaviorPlayedCount;

	// 이번 Tick을 실행하는 프레임이 새로운 behavior를 시작하는 프레임인지에 대한 여부를 얻습니다.
	// 새로운 behavior를 시작할 때마다 최초 1회에 한해 초기화되어야 하는 값을 초기화하기 위함입니다.
	const bool _behaviorStart = _previousPlayedCount < _behaviorPlayedCount;
	_previousPlayedCount = _behaviorPlayedCount;

	if (_behaviorPlayedCount >= PATTERN1_BEHAVIOR_LOOP_COUNT)
	{
		// 패턴을 모두 재생했을 때 패턴을 종료합니다.

		_boss->visibleWorldObject = false;
		_boss->visibleWarningSign = false;
		_boss->visibleMoon = false;

		return true;
	}

	// 제 1사분면부터 제 4사분면까지 목적지 값을 초기화합니다.

	const float _gapFromScreen = 20;
	const Vector2D _destinationArr[4] = {
		{_dodgeGameInstance->screenWidth - _gapFromScreen, _dodgeGameInstance->screenHeight - _gapFromScreen},
		{_gapFromScreen, _dodgeGameInstance->screenHeight - _gapFromScreen},
		{_gapFromScreen, _gapFromScreen},
		{_dodgeGameInstance->screenWidth - _gapFromScreen, _gapFromScreen}
	};

	const Vector2D _warningSignPositionArr[4] = {
		{_dodgeGameInstance->screenWidth - _gapFromScreen, _dodgeGameInstance->screenHeight - _gapFromScreen},
		{_gapFromScreen, _dodgeGameInstance->screenHeight - _gapFromScreen},
		{_gapFromScreen, _gapFromScreen},
		{_dodgeGameInstance->screenWidth - _gapFromScreen, _gapFromScreen}
	};

	const Vector2D _pivotArr[4] = {
		{1, 1},
		{0, 1},
		Vector2D_Zero,
		Vector2D_Right
	};

	const int _muskImageWidth = _boss->worldObject->sprite->imageWidth;
	const int _muskImageHeight = _boss->worldObject->sprite->imageHeight;

	const Vector2D _muskStartPositionArr[4] = {
		{_dodgeGameInstance->screenWidth + _muskImageWidth / 2, _dodgeGameInstance->screenHeight + _muskImageHeight / 2},
		{-_muskImageWidth / 2, _dodgeGameInstance->screenHeight + _muskImageHeight / 2},
		{-_muskImageWidth / 2, -_muskImageHeight},
		{_dodgeGameInstance->screenWidth + _muskImageWidth / 2, -_muskImageHeight}
	};

	static int _latestDestinationIndex = -1;

	if (_behaviorStart == true)
	{
		
	}

	if (_behaviorElapsedTime < PATTERN1_NODE1_DURATION)
	{
		// # BEHAVIOR 1
		// 주의 표시가 1초간 깜빡이며 표시됩니다.

		const bool _blinkState = (int)(_patternElapsedTime * 10) % 2 == 0;
		_boss->visibleWarningSign = _blinkState;
	}
	else if (_behaviorElapsedTime < PATTERN1_NODE1_DURATION + PATTERN1_NODE2_DURATION)
	{
		// # BEHAVIOR 2
		// 머스크가 달을 향해 날아갑니다.

		// 머스크를 화면에 표시하고,
		// 주의 표시를 더 이상 표시하지 않습니다.
		_boss->visibleWorldObject = true;
		_boss->visibleWarningSign = false;

		// 이번 프레임의 위치를 새로고칩니다.
		const Vector2D _destination = _destinationArr[_latestDestinationIndex];
		const Vector2D _nextPosition = Vector2D_Lerp(_boss->worldObject->position, _destination, 5 * _deltaTime);
		_boss->worldObject->position = _nextPosition;
	}
	else if (_behaviorElapsedTime < PATTERN1_NODE1_DURATION + PATTERN1_NODE2_DURATION + PATTERN1_NODE3_DURATION)
	{
		// # BEHAVIOR 3
		// 쉬어가는 시간입니다.
	}

	return false;
}

bool _Boss_Kkanbu_Pattern1Behavior1Tick(struct DodgeGameInstance* _dodgeGameInstance, Boss_Kkanbu* _boss, double _deltaTime, bool _behaviorStart)
{
	if(_behaviorStart == true)
	{
		// behavior를 시작할 때 1회에 한해 초기화합니다.
	}
}
