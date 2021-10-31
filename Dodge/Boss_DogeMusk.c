#include "Boss_DogeMusk.h"

#include <math.h>
#include <stdlib.h>

#include "DodgeGameInstance.h"
#include "Player.h"
#include "Scene_Game.h"


Boss_DogeMusk* Boss_DogeMusk_Create(GameInstance* _gameInstance)
{
	Boss_DogeMusk* _outBoss = (Boss_DogeMusk*)malloc(sizeof(Boss_DogeMusk));

	// boss object
	_outBoss->visibleWorldObject = false;

	const Vector2D _position = {_gameInstance->screen->width / 2, _gameInstance->screen->height * 0.75f};
	const Vector2D _pivot = Vector2D_Center;
	_outBoss->worldObject = WorldObject_CreateWithSpriteMask(L"Sprites/boss_dogemusk/boss_dogemusk.txt", L"Sprites/boss_dogemusk/boss_dogemusk.txt",
	                                                         L"Sprites/boss_dogemusk/boss_dogemusk.txt", _pivot, _position);

	// warningSignObject
	_outBoss->visibleWarningSign = false;
	_outBoss->warningSignObject = WorldObject_CreateWithSpriteMask(L"Sprites/test_warning.txt", L"Sprites/test_warning.txt", L"Sprites/test_warning.txt",
	                                                               Vector2D_Center, Vector2D_Zero);

	// moon
	_outBoss->visibleMoon = false;
	_outBoss->moonObject = WorldObject_CreateWithSpriteMask(L"Sprites/boss_dogemusk/boss_dogemusk_fullmoon.txt",
	                                                        L"Sprites/boss_dogemusk/boss_dogemusk_fullmoon.txt",
	                                                        L"Sprites/boss_dogemusk/boss_dogemusk_fullmoon.txt",
	                                                        Vector2D_Center, Vector2D_Zero);

	_outBoss->bossStartTime = GameInstance_GetGameTime(_gameInstance);

	// state
	_outBoss->currentState = EBDMS_START;

	// patterns
	_outBoss->currentPatternType = EBDMPT_NONE;
	_outBoss->previousPatternType = EBDMPT_NONE;
	_outBoss->patternStartTime = 0;
	_outBoss->patternEndTime = 0;


	_outBoss->dogeMuskClip = AudioClip_LoadFromFile(_gameInstance->audio, L"sounds/sfx/dogemusk.wav", false);


	return _outBoss;
}

void Boss_DogeMusk_Release(Boss_DogeMusk* _boss)
{
	if (_boss == NULL)
		return;

	if (_boss->worldObject != NULL)
		WorldObject_Release(_boss->worldObject);

	if (_boss->warningSignObject != NULL)
		WorldObject_Release(_boss->warningSignObject);

	if (_boss->warningSignObject != NULL)
		AudioClip_Release(_boss->dogeMuskClip);

	free(_boss);
}

bool _Boss_DogeMusk_ShouldStartNewPattern(GameInstance* _gameInstance, Boss_DogeMusk* _boss)
{
	if (_gameInstance == NULL || _boss == NULL)
		return false;

#define START_DELAY 2.f
#define PATTERN_DELAY 1.f

	const double _gameTime = GameInstance_GetGameTime(_gameInstance);

	switch (_boss->currentState)
	{
	case EBDMS_START:
		{
			// 보스가 시작되었을 때 일정 시간이 흐른 뒤에 새로운 패턴을 재생합니다.
			const double _elapsedTimeAfterBossStart = _gameTime - _boss->bossStartTime;
			const bool _shouldStartNewPattern = _elapsedTimeAfterBossStart >= START_DELAY;

			return _shouldStartNewPattern;
		}

	case EBDMS_PATTERN_DELAYED:
		{
			// 패턴을 재생하고 난 뒤 일정 시간이 흐른 뒤에 새로운 패턴을 재생합니다.
			const double _elapsedTimeAfterPatternEnd = _gameTime - _boss->patternEndTime;
			const bool _shouldStartNewPattern = _elapsedTimeAfterPatternEnd >= PATTERN_DELAY;

			return _shouldStartNewPattern;
		}
	case EBDMS_PATTERN_PLAYING:
		// 이미 패턴을 재생하고 있을 때에는 새로운 패턴을 재생하지 않습니다.
		return false;
	}

	return false;
}

void Boss_DogeMusk_DrawTick(GameInstance* _gameInstance, Boss_DogeMusk* _boss)
{
	if (_gameInstance == NULL || _boss == NULL)
		return;

	const Screen* _screen = _gameInstance->screen;

	if (_boss->warningSignObject != NULL && _boss->visibleWarningSign == true)
		Screen_PrintWorldObject(_screen, _boss->warningSignObject);

	if (_boss->moonObject != NULL && _boss->visibleMoon == true)
		Screen_PrintWorldObject(_screen, _boss->moonObject);

	if (_boss->worldObject != NULL && _boss->visibleWorldObject == true)
		Screen_PrintWorldObject(_screen, _boss->worldObject);
}

void Boss_DogeMusk_CollisionTick(DodgeGameInstance* _dodgeGameInstance, Boss_DogeMusk* _boss)
{
	if (_dodgeGameInstance == NULL || _boss == NULL || _dodgeGameInstance->gameScene == NULL)
		return;

	const Player* _player = _dodgeGameInstance->gameScene->player;

	if (_player == NULL)
		return;

	const Collider* _dogeMuskCollider = _boss->worldObject->collider;
	const Vector2D _dogeMuskPosition = _boss->worldObject->position;
	const Vector2D _dogeMuskPivot = _boss->worldObject->pivot;

	const Collider* _playerCollider = _player->worldObject->collider;
	const Vector2D _playerPosition = _player->worldObject->position;
	const Vector2D _playerPivot = _player->worldObject->pivot;

	const bool _collisionResult = Collider_CheckCollision(_dogeMuskCollider, _dogeMuskPosition, _dogeMuskPivot,
	                                                      _playerCollider, _playerPosition, _playerPivot);

	if (_collisionResult && _player->isInvincible == false)
	{
		const double _gameTime = GameInstance_GetGameTime(_dodgeGameInstance->gameInstance);

		Player_Damaged(_dodgeGameInstance, _player, 1, _gameTime);
	}
}

void Boss_DogeMusk_Tick(DodgeGameInstance* _dodgeGameInstance, Boss_DogeMusk* _boss, double _deltaTime)
{
	if (_dodgeGameInstance == NULL || _boss == NULL)
		return;

	const bool _startNewPattern = _Boss_DogeMusk_ShouldStartNewPattern(_dodgeGameInstance->gameInstance, _boss);

	if (_startNewPattern == true)
	{
		// 패턴을 재생할 때가 되었을 때,
		// 패턴 관련 변수의 값들을 설정하고 패턴을 시작합니다.
		_Boss_DogeMusk_PrePattern(_dodgeGameInstance->gameInstance, _boss);
	}

	bool _patternEnd = false;

	switch (_boss->currentState)
	{
		// 현재 패턴을 재생합니다.
		// 만약 재생중인 패턴이 없다면 어떠한 동작도 하지 않습니다.
	case EBDMS_PATTERN_PLAYING:
		_patternEnd = _Boss_DogeMusk_Pattern1Tick(_dodgeGameInstance->gameInstance, _boss, _deltaTime, _startNewPattern);
		break;
	}

	if (_patternEnd == true)
	{
		// 패턴이 끝난 직후라면, PostPattern을 호출합니다.
		_Boss_DogeMusk_PostPattern(_dodgeGameInstance->gameInstance, _boss);
	}
}

void _Boss_DogeMusk_PrePattern(GameInstance* _gameInstance, Boss_DogeMusk* _boss)
{
	if (_gameInstance == NULL || _boss == NULL)
		return;

#define PATTERN_COUNT 1

	const double _gameTime = GameInstance_GetGameTime(_gameInstance);

	// 패턴을 시작하기 앞서 변수들을 초기화합니다.
	_boss->patternStartTime = _gameTime;
	_boss->currentState = EBDMS_PATTERN_PLAYING;

	// 이번에 재생할 패턴의 유형을 선택합니다.
	// 패턴의 개수가 2개 이상일 때,
	// 가장 마지막으로 재생했던 패턴의 유형은 다시 선택하지 않습니다.
	do { _boss->currentPatternType = (EBoss_DogeMusk_PatternType)(rand() % PATTERN_COUNT + 1); }
	while (PATTERN_COUNT >= 2 && _boss->currentPatternType == _boss->previousPatternType);
}

void _Boss_DogeMusk_PostPattern(GameInstance* _gameInstance, Boss_DogeMusk* _boss)
{
	if (_gameInstance == NULL || _boss == NULL)
		return;

	const double _gameTime = GameInstance_GetGameTime(_gameInstance);

	// 패턴이 종료된 이후 변수들을 초기화합니다.

	_boss->patternEndTime = _gameTime;

	_boss->previousPatternType = _boss->currentPatternType;
	_boss->currentState = EBDMS_PATTERN_DELAYED;
	_boss->currentPatternType = EBDMS_NONE;
}

bool _Boss_DogeMusk_Pattern1Tick(GameInstance* _gameInstance, Boss_DogeMusk* _boss, double _deltaTime, bool _patternStart)
{
#define PATTERN1_NODE1_DURATION .6f
#define PATTERN1_NODE2_DURATION 1.f
#define PATTERN1_NODE3_DURATION 0.f

#define PATTERN1_BEHAVIOR_LOOP_COUNT 5

	static int _previousPlayedCount = -1;

	if (_patternStart)
	{
		// pattern을 시작할 때 1회에 한해 초기화합니다.
		_previousPlayedCount = -1;
	}

	const float _behaviorDuration = PATTERN1_NODE1_DURATION + PATTERN1_NODE2_DURATION + PATTERN1_NODE3_DURATION;

	const double _patternElapsedTime = GameInstance_GetGameTime(_gameInstance) - _boss->patternStartTime;

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

	const int _screenWidth = _gameInstance->screen->width;
	const int _screenHeight = _gameInstance->screen->height;

	const float _gapFromScreen = 20;
	const Vector2D _destinationArr[4] = {
		{_screenWidth - _gapFromScreen, _screenHeight - _gapFromScreen},
		{_gapFromScreen, _screenHeight - _gapFromScreen},
		{_gapFromScreen, _gapFromScreen},
		{_screenWidth - _gapFromScreen, _gapFromScreen}
	};

	const Vector2D _warningSignPositionArr[4] = {
		{_screenWidth - _gapFromScreen, _screenHeight - _gapFromScreen},
		{_gapFromScreen, _screenHeight - _gapFromScreen},
		{_gapFromScreen, _gapFromScreen},
		{_screenWidth - _gapFromScreen, _gapFromScreen}
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
		{_screenWidth + _muskImageWidth / 2, _screenHeight + _muskImageHeight / 2},
		{-_muskImageWidth / 2, _screenHeight + _muskImageHeight / 2},
		{-_muskImageWidth / 2, -_muskImageHeight},
		{_screenWidth + _muskImageWidth / 2, -_muskImageHeight}
	};

	static int _latestDestinationIndex = -1;

	if (_behaviorStart == true)
	{
		// behavior를 시작할 때 1회에 한해 초기화합니다.
		// 달의 위치와 머스크의 위치를 임의로 결정합니다.
		// 단, 바로 이전의 behavior에서 결정되었던 목적지로 다시 결정되지는 않습니다.
		int _targetDestinationIndex = 0;
		do { _targetDestinationIndex = rand() % 4; }
		while (_targetDestinationIndex == _latestDestinationIndex);
		_latestDestinationIndex = _targetDestinationIndex;


		// warning sign과 musk는 달의 반대편에서 생성됩니다.
		const int _oppositeSideIndex = (_targetDestinationIndex + 2) % 4;

		// warning sign의 위치와 중심점을 갱신합니다.
		const Vector2D _warningSignPosition = _warningSignPositionArr[_oppositeSideIndex];
		const Vector2D _warningSignPivot = _pivotArr[_oppositeSideIndex];
		_boss->warningSignObject->position = _warningSignPosition;
		_boss->warningSignObject->pivot = _warningSignPivot;

		// musk의 시작 위치를 갱신합니다.
		const Vector2D _muskStartPosition = _muskStartPositionArr[_oppositeSideIndex];
		_boss->worldObject->position = _muskStartPosition;

		// moon의 시작 위치를 갱신하고 표시합니다.
		const Vector2D _moonPosition = _destinationArr[_targetDestinationIndex];
		const Vector2D _moonPivot = _pivotArr[_targetDestinationIndex];
		_boss->moonObject->position = _moonPosition;
		_boss->moonObject->pivot = _moonPivot;
		_boss->visibleMoon = true;

		Audio_Play(_gameInstance->audio, _boss->dogeMuskClip, true);
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
