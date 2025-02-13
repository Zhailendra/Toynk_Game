// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/ToynkGameInstance.h"

// GET

int UToynkGameInstance::GetKillCount()
{
	return KillCount;
}

int UToynkGameInstance::GetLevel()
{
	return Level;
}

int UToynkGameInstance::GetCoins()
{
	return Coins;
}

int UToynkGameInstance::GetBulletRicochet()
{
	return BulletRicochet;
}

float UToynkGameInstance::GetBulletVelocity()
{
	return BulletVelocity;
}

int UToynkGameInstance::GetMoveSpeed()
{
	return MoveSpeed;
}

int UToynkGameInstance::GetHealth()
{
	return Health;
}

float UToynkGameInstance::GetCoinRate()
{
	return CoinRate;
}

int UToynkGameInstance::GetMaxMines()
{
	return MaxMines;
}

// SET

void UToynkGameInstance::IncrementKillCount()
{
	KillCount++;
}

void UToynkGameInstance::GoToNextLevel()
{
	Level++;
}

void UToynkGameInstance::GoToLevel(int _Level)
{
	Level = _Level;
}

void UToynkGameInstance::AddCoins(int _amount)
{
	Coins += _amount;
}

void UToynkGameInstance::RemoveCoins(int _amount)
{
	Coins -= _amount;
}

void UToynkGameInstance::UpgradeBulletRicochet(int _amount)
{
	BulletRicochet += _amount;
}

void UToynkGameInstance::UpgradeBulletVelocity(float _amount)
{
	BulletVelocity += _amount;
}

void UToynkGameInstance::UpgradeMoveSpeed(int _amount)
{
	MoveSpeed += _amount;
}

void UToynkGameInstance::UpgradeHealth(int _amount)
{
	Health += _amount;
}

void UToynkGameInstance::UpgradeCoinRate(float _amount)
{
	CoinRate += _amount;
}

void UToynkGameInstance::UpgradeMaxMines(int _amount)
{
	MaxMines = _amount;
}
