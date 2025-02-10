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
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Data")
	int tankAmount = 0;
};
