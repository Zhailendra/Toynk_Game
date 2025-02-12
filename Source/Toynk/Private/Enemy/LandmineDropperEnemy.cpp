#include "Enemy/LandmineDropperEnemy.h"

ALandmineDropperEnemy::ALandmineDropperEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ALandmineDropperEnemy::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_DropMineCooldown, this, &ABaseTank::DropLandMine, DropMineCooldown, true);
}

void ALandmineDropperEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
