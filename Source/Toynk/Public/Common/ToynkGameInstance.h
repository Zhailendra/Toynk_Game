// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ToynkGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TOYNK_API UToynkGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	int GetKillCount();

	UFUNCTION(BlueprintCallable)
	int GetLevel();

	UFUNCTION(BlueprintCallable)
	void IncrementKillCount();

	UFUNCTION(BlueprintCallable)
	void GoToNextLevel();

	UFUNCTION(BlueprintCallable)
	void GoToLevel(int _level);
	
private:
	UPROPERTY(EditAnywhere, Category = "Game Data")
	int killCount = 0;

	UPROPERTY(EditAnywhere, Category = "Game Data")
	int level = 0;
};
