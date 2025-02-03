// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Base_Level.h"

// Sets default values
ABase_Level::ABase_Level()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABase_Level::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABase_Level::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

