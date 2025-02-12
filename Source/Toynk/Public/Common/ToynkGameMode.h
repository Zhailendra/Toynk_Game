// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ToynkGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TOYNK_API AToynkGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	int GetEnemyCount();

	UFUNCTION(BlueprintCallable)
	void SetEnemyCount(int _enemyCount);

private:
	int enemyCount = 0;
};
