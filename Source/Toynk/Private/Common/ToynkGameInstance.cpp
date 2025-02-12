// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/ToynkGameInstance.h"

int UToynkGameInstance::GetKillCount()
{
	return killCount;
}

int UToynkGameInstance::GetLevel()
{
	return level;
}

void UToynkGameInstance::IncrementKillCount()
{
	killCount++;
}

void UToynkGameInstance::GoToNextLevel()
{
	level++;
}

void UToynkGameInstance::GoToLevel(int _level)
{
	level = _level;
}
