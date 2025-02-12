// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/ToynkGameMode.h"

int AToynkGameMode::GetEnemyCount()
{
	return enemyCount;
}

void AToynkGameMode::SetEnemyCount(int _enemyCount)
{
	enemyCount = _enemyCount;
}
