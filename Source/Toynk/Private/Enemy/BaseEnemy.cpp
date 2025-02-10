#include "Enemy/BaseEnemy.h"

#include "Tank/PlayerTank.h"

#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (ToynkGameInstance)
	{
		ToynkGameInstance->enemyLeft += 1;
	}

	PlayerTank = Cast<APlayerTank>(UGameplayStatics::GetPlayerPawn(this, 0));

	GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ABaseEnemy::CheckFireCondition, FireRate, true);
}

void ABaseEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (InFireRange())
	{
		RotateToCursor(PlayerTank->GetActorLocation());
	}
}

void ABaseEnemy::CheckFireCondition()
{
	if (InFireRange())
	{
		Fire();
	}

}

bool ABaseEnemy::InFireRange()
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
