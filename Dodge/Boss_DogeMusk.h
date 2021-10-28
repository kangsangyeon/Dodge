#pragma once
#include "Vector2D.h"
#include "WorldObject.h"

typedef enum TEBoss_DogeMusk_State
{
	EBDPS_NONE = 0,
	EBDPS_START = 1,
	EBDPT_PATTERN_PLAYING = 2,
	EBDPS_PATTERN_DELAYED = 3
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
} Boss_DogeMusk;

Boss_DogeMusk* Boss_DogeMusk_Create(struct DodgeGameInstance* _dodgeGameInstance);

void Boss_DogeMusk_Release(Boss_DogeMusk* _boss);

void Boss_DogeMusk_DrawTick(struct DodgeGameInstance* _dodgeGameInstance, Boss_DogeMusk* _boss);

void Boss_DogeMusk_Tick(struct DodgeGameInstance* _dodgeGameInstance, Boss_DogeMusk* _boss, double _deltaTime);

bool _Boss_DogeMusk_ShouldStartNewPattern(struct DodgeGameInstance* _dodgeGameInstance, Boss_DogeMusk* _boss);

void _Boss_DogeMusk_PrePattern(struct DodgeGameInstance* _dodgeGameInstance, Boss_DogeMusk* _boss);

void _Boss_DogeMusk_PostPattern(struct DodgeGameInstance* _dodgeGameInstance, Boss_DogeMusk* _boss);

bool _Boss_DogeMusk_Pattern1Tick(struct DodgeGameInstance* _dodgeGameInstance, Boss_DogeMusk* _boss, double _deltaTime, bool _patternStart);
