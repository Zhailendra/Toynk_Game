#include "Tank/BaseTank.h"

#include "Tank/Bullet/Bullet.h"
#include "Tank/LandMine/LandMine.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/HealthComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "ObjectPoolIng/PoolSubsystem.h"

ABaseTank::ABaseTank()
{
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add(FName("DestroyableActor"));


	BaseMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMeshComponent"));
	BaseMeshComponent->SetupAttachment(RootComponent);

	TurretMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMeshComponent"));
	TurretMeshComponent->SetupAttachment(RootComponent);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);

	ForwardArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ForwardArrowComponent"));
	ForwardArrowComponent->SetupAttachment(RootComponent);

	RightArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("RightArrowComponent"));
	RightArrowComponent->SetupAttachment(RootComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(TurretMeshComponent);

	LandMineSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("LandMineSpawnPoint"));
	LandMineSpawnPoint->SetupAttachment(BaseMeshComponent);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

void ABaseTank::BeginPlay()
{
	Super::BeginPlay();

	PC = Cast<APlayerController>(GetController());
	PoolSubsystem = GetWorld()->GetSubsystem<UPoolSubsystem>();
	CurrentLandMines = 0;

	if (PC)
	{
		PC->bShowMouseCursor = true;
		PC->bEnableClickEvents = true;
		PC->bEnableMouseOverEvents = true;
		PC->CurrentMouseCursor = EMouseCursor::Crosshairs;
	}
}

void ABaseTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PC)
	{
		FHitResult HitResult;
		 bool bHit = PC->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

		if (bHit)
		{
			RotateToCursor(HitResult.ImpactPoint);
		} else
		{
			DrawDebugLine(GetWorld(), TurretMeshComponent->GetComponentLocation(), HitResult.TraceEnd, FColor::Red, false, 0.1f, 0, 1.0f);
		}
	}

	RotateBodyTowardsMovementDirection();
}

void ABaseTank::RotateToCursor(const FVector& LookAtTarget) const
{
	const FVector ToTarget = LookAtTarget - TurretMeshComponent->GetComponentLocation();
	const FRotator LookAtRotation = FRotator(0.0f, ToTarget.Rotation().Yaw + 95.f, 0.0f);

	TurretMeshComponent->SetWorldRotation(
		FMath::RInterpTo(TurretMeshComponent->GetComponentRotation(), LookAtRotation, GetWorld()->GetDeltaSeconds(), InterpSpeed),
		true
	);
}

void ABaseTank::RotateBodyTowardsMovementDirection() const
{
	FVector Velocity = GetVelocity();

	if (!Velocity.IsNearlyZero())
	{
		FVector CurrentLocation = GetActorLocation();
		FVector NewLocation = CurrentLocation + Velocity;

		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, NewLocation);

		FRotator AdjustedRotation = FRotator(
			LookAtRotation.Pitch,
			LookAtRotation.Yaw + 90.0f,
			LookAtRotation.Roll
		);

		FRotator NewRotation = FMath::RInterpTo(
			BaseMeshComponent->GetComponentRotation(),
			AdjustedRotation,
			GetWorld()->GetDeltaSeconds(),
			2.5f
		);

		BaseMeshComponent->SetWorldRotation(NewRotation);
	}
}

void ABaseTank::Fire()
{
	if (!bCanFire) return;

	bCanFire = false;

	if (Controller && PoolSubsystem)
	{
		PoolSubsystem->SpawnFromPool<ABullet>(
			BulletClass,
			ProjectileSpawnPoint->GetComponentLocation(),
			ProjectileSpawnPoint->GetComponentRotation() - FRotator(0, 90, 0),
			this
		)->InitBullet(this);
	}

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_FireCooldown, this, &ABaseTank::ResetFire, FireCooldown, false);
}

void ABaseTank::ResetFire()
{
	bCanFire = true;
}

void ABaseTank::DropLandMine()
{
	if (!bCanDropMine) return;

	bCanDropMine = false;

	if (Controller && PoolSubsystem && CurrentLandMines < MaxLandMines)
	{
		CurrentLandMines++;
		PoolSubsystem->SpawnFromPool<ALandMine>(
			LandMineClass,
			LandMineSpawnPoint->GetComponentLocation(),
			LandMineSpawnPoint->GetComponentRotation(),
			this
		)->InitLandMine(this);
	}
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_DropMineCooldown, this, &ABaseTank::ResetDropMine, DropMineCooldown, false);
}

void ABaseTank::ResetDropMine()
{
	bCanDropMine = true;
}
