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

#include "DrawDebugHelpers.h"

bool ATower::InFireRange()
{
	if (!PlayerTank)
	{
		return false;
	}

	float Distance = FVector::Dist(GetActorLocation(), PlayerTank->GetActorLocation());
	if (Distance > FireRange)
	{
		return false;
	}

	FVector Start = GetActorLocation();
	FVector End = PlayerTank->GetActorLocation();

	TArray<FHitResult> HitResults;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	CollisionParams.AddIgnoredActor(PlayerTank);

	bool bHit = GetWorld()->LineTraceMultiByChannel(
		HitResults,
		Start,
		End,
		ECC_Visibility,
		CollisionParams
	);

	//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 2.0f);

	if (!bHit)
	{
		return true;
	}

	for (const FHitResult& Hit : HitResults)
	{
		AActor* HitActor = Hit.GetActor();

		if (HitActor && HitActor != PlayerTank)
		{
			return false;
		}
	}

	return true;
}
