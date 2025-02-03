#include "Tower/Tower.h"
#include "Tank/TankBody.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void ATower::BeginPlay()
{
	Super::BeginPlay();

	PlayerTank = Cast<ATankBody>(UGameplayStatics::GetPlayerPawn(this, 0));

	GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);
}

void ATower::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (InFireRange())
	{
		RotateTurret(PlayerTank->GetActorLocation());
	}
}

void ATower::CheckFireCondition()
{
	if (InFireRange())
	{
		Fire();
	}

}

bool ATower::InFireRange()
{
	if (PlayerTank)
	{
		float Distance = FVector::Dist(GetActorLocation(), PlayerTank->GetActorLocation());
		if (Distance <= FireRange)
		{
			return true;
		}
	}
	return false;
}