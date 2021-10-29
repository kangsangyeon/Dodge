#pragma once
#include "WorldObject.h"

typedef struct TAudioClip AudioClip;

typedef enum TEBoss_Kkanbu_State
{
	EBKS_NONE = 0,
	EBKS_START = 1,
	EBKS_PATTERN_PLAYING = 2,
	EBKS_PATTERN_DELAYED = 3
} EBoss_Kkanbu_State;

typedef enum TEBoss_Kkanbu_PatternType
{
	EBKPT_NONE = 0,
	EBKPT_PATTERN1 = 1,
} EBoss_Kkanbu_PatternType;

typedef struct TBoss_Kkanbu
{
	WorldObject* worldObject;
	bool visibleWorldObject;

	double bossStartTime;

	EBoss_Kkanbu_State currentState;
	EBoss_Kkanbu_PatternType currentPatternType;
	EBoss_Kkanbu_PatternType previousPatternType;
	double patternStartTime;
	double patternEndTime;
} Boss_Kkanbu;

Boss_Kkanbu* Boss_Kkanbu_Create(struct DodgeGameInstance* _dodgeGameInstance);

void Boss_Kkanbu_Release(Boss_Kkanbu* _boss);

void Boss_Kkanbu_DrawTick(struct DodgeGameInstance* _dodgeGameInstance, Boss_Kkanbu* _boss);

void Boss_Kkanbu_Tick(struct DodgeGameInstance* _dodgeGameInstance, Boss_Kkanbu* _boss, double _deltaTime);

bool _Boss_Kkanbu_ShouldStartNewPattern(struct DodgeGameInstance* _dodgeGameInstance, Boss_Kkanbu* _boss);

void _Boss_Kkanbu_PrePattern(struct DodgeGameInstance* _dodgeGameInstance, Boss_Kkanbu* _boss);

void _Boss_Kkanbu_PostPattern(struct DodgeGameInstance* _dodgeGameInstance, Boss_Kkanbu* _boss);

bool _Boss_Kkanbu_Pattern1Tick(struct DodgeGameInstance* _dodgeGameInstance, Boss_Kkanbu* _boss, double _deltaTime, bool _patternStart);

bool _Boss_Kkanbu_Pattern1Behavior1Tick(struct DodgeGameInstance* _dodgeGameInstance, Boss_Kkanbu* _boss, double _deltaTime, bool _patternStart);
