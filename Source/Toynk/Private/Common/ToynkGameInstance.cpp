// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/ToynkGameInstance.h"

int UToynkGameInstance::GetKillCount()
{
	return killCount;
}

void UToynkGameInstance::IncrementKillCount()
{
	killCount++;
}
