#include "Enemy/LandmineDropperEnemy.h"

ALandmineDropperEnemy::ALandmineDropperEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ALandmineDropperEnemy::BeginPlay()
{
	Super::BeginPlay();

	bIsNotFollowingPlayer = true;
}

void ALandmineDropperEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DropLandMine();
}
