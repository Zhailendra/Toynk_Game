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
	// GET

	UFUNCTION(BlueprintCallable)
	int GetKillCount();

	UFUNCTION(BlueprintCallable)
	int GetLevel();

	UFUNCTION(BlueprintCallable)
	int GetCoins();

	UFUNCTION(BlueprintCallable)
	int GetBulletRicochet();

	UFUNCTION(BlueprintCallable)
	float GetBulletVelocity();

	UFUNCTION(BlueprintCallable)
	int GetMoveSpeed();

	UFUNCTION(BlueprintCallable)
	int GetHealth();

	UFUNCTION(BlueprintCallable)
	float GetCoinRate();

	UFUNCTION(BlueprintCallable)
	int GetMaxMines();

	// SET

	UFUNCTION(BlueprintCallable)
	void IncrementKillCount();

	UFUNCTION(BlueprintCallable)
	void GoToNextLevel();

	UFUNCTION(BlueprintCallable)
	void GoToLevel(int _level);

	UFUNCTION(BlueprintCallable)
	void AddCoins(int _amount);

	UFUNCTION(BlueprintCallable)
	void RemoveCoins(int _amount);

	UFUNCTION(BlueprintCallable)
	void UpgradeBulletRicochet(int _amount = 1);

	UFUNCTION(BlueprintCallable)
	void UpgradeBulletVelocity(float _amount = 100.f);

	UFUNCTION(BlueprintCallable)
	void UpgradeMoveSpeed(int _amount = 100);

	UFUNCTION(BlueprintCallable)
	void UpgradeHealth(int _amount = 50);

	UFUNCTION(BlueprintCallable)
	void UpgradeCoinRate(float _amount = 0.1f);

	UFUNCTION(BlueprintCallable)
	void UpgradeMaxMines(int _amount = 5);
	
private:
	// GAME DATA

	UPROPERTY(EditAnywhere, Category = "Game Data")
	int KillCount = 0;

	UPROPERTY(EditAnywhere, Category = "Game Data")
	int Level = 0;

	UPROPERTY(EditAnywhere, Category = "Game Data")
	int Coins = 0;

	// UPGRADES

	UPROPERTY(EditAnywhere, Category = "Upgrades")
	int BulletRicochet = 1;

	UPROPERTY(EditAnywhere, Category = "Upgrades")
	float BulletVelocity = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Upgrades")
	int MoveSpeed = 500;

	UPROPERTY(EditAnywhere, Category = "Upgrades")
	int Health = 100;

	UPROPERTY(EditAnywhere, Category = "Upgrades")
	float CoinRate = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Upgrades")
	int MaxMines = 5;
};
