#include "Common/BasePawn.h"

#include "ObjectPoolIng/PoolSubsystem.h"
#include "Bullet/Bullet.h"
#include "LandMine/LandMine.h"
#include "Components/BoxComponent.h"

ABasePawn::ABasePawn()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;

	BaseMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMeshComponent"));
	BaseMeshComponent->SetupAttachment(BoxComponent);

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SceneComponent->SetupAttachment(BaseMeshComponent);

	TurretMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMeshComponent"));
	TurretMeshComponent->SetupAttachment(SceneComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(TurretMeshComponent);

	LandMineSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("LandMineSpawnPoint"));
	LandMineSpawnPoint->SetupAttachment(SceneComponent);
}

void ABasePawn::BeginPlay()
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

void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (PC)
	{
		FHitResult HitResult;
		PC->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

		//DrawDebugLine(GetWorld(), TurretMeshComponent->GetComponentLocation(), HitResult.ImpactPoint, FColor::Red, false, 0.1f, 0, 1.0f);

		RotateTurret(HitResult.ImpactPoint);
	}
}

void ABasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABasePawn::RotateTurret(const FVector& LookAtTarget) const
{
	FVector ToTarget = LookAtTarget - TurretMeshComponent->GetComponentLocation();
	FRotator LookAtRotation = FRotator(0.0f, ToTarget.Rotation().Yaw + 90, 0.0f);
	
	TurretMeshComponent->SetWorldRotation(
		FMath::RInterpTo(TurretMeshComponent->GetComponentRotation(), LookAtRotation, GetWorld()->GetDeltaSeconds(), InterpSpeed),
		true
	);
}

void ABasePawn::Fire()
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

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_FireCooldown, this, &ABasePawn::ResetFire, FireCooldown, false);
}

void ABasePawn::ResetFire()
{
	bCanFire = true;
}

void ABasePawn::DropLandMine()
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
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_DropMineCooldown, this, &ABasePawn::ResetDropMine, DropMineCooldown, false);
}

void ABasePawn::ResetDropMine()
{
	bCanDropMine = true;
}
