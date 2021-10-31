#pragma once
#include "Collider.h"
#include "Vector2D.h"
#include "WorldObject.h"
#include "AudioClip.h"

typedef enum TEBoss_DogeMusk_State
{
	EBDMS_NONE = 0,
	EBDMS_START = 1,
	EBDMS_PATTERN_PLAYING = 2,
	EBDMS_PATTERN_DELAYED = 3
} EBoss_DogeMusk_State;

typedef enum TEBoss_DogeMusk_PatternType
{
	EBDMPT_NONE = 0,
	EBDMPT_PATTERN1 = 1,
} EBoss_DogeMusk_PatternType;

typedef struct TBoss_DogeMusk
{
	WorldObject* worldObject;
	bool visibleWorldObject;

	WorldObject* warningSignObject;
	bool visibleWarningSign;

	WorldObject* moonObject;
	bool visibleMoon;

	double bossStartTime;

	EBoss_DogeMusk_State currentState;
	EBoss_DogeMusk_PatternType currentPatternType;
	EBoss_DogeMusk_PatternType previousPatternType;
	double patternStartTime;
	double patternEndTime;

	AudioClip* dogeMuskClip;
} Boss_DogeMusk;

Boss_DogeMusk* Boss_DogeMusk_Create(struct GameInstance* _gameInstance);

void Boss_DogeMusk_Release(Boss_DogeMusk* _boss);

void Boss_DogeMusk_DrawTick(struct GameInstance* _gameInstance, Boss_DogeMusk* _boss);

void Boss_DogeMusk_CollisionTick(struct DodgeGameInstance* _dodgeGameInstance, Boss_DogeMusk* _boss);

void Boss_DogeMusk_Tick(struct DodgeGameInstance* _dodgeGameInstance, Boss_DogeMusk* _boss, double _deltaTime);

bool _Boss_DogeMusk_ShouldStartNewPattern(struct GameInstance* _gameInstance, Boss_DogeMusk* _boss);

void _Boss_DogeMusk_PrePattern(struct GameInstance* _gameInstance, Boss_DogeMusk* _boss);

void _Boss_DogeMusk_PostPattern(struct GameInstance* _gameInstance, Boss_DogeMusk* _boss);

bool _Boss_DogeMusk_Pattern1Tick(struct GameInstance* _gameInstance, Boss_DogeMusk* _boss, double _deltaTime, bool _patternStart);
