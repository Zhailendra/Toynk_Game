#include "Tank/BaseTank.h"

#include "Tank/Bullet/Bullet.h"
#include "Tank/LandMine/LandMine.h"
#include "Common/ToynkGameMode.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/HealthComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "ObjectPoolIng/PoolSubsystem.h"
#include "Sound/SoundCue.h"
#include "NiagaraFunctionLibrary.h"

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

	ToynkGameInstance = Cast<UToynkGameInstance>(GetGameInstance());
	ToynkGameMode = Cast<AToynkGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

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
		ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1);
		FHitResult HitResult;
		bool bHit = PC->GetHitResultUnderCursorByChannel(TraceType, false, HitResult);

		if (bHit)
		{
			RotateToCursor(HitResult.Location);
		} else
		{
			DrawDebugLine(GetWorld(), GetMesh()->GetComponentLocation(), HitResult.Location, FColor::Red, false, 0.1f, 0, 1.0f);
		}

		if (GetVelocity().Size() > 0)
		{
			OldTick += DeltaTime;

			if (OldTick > 0.1f)
			{
				if (ChainSound != nullptr)
				{
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), ChainSound, GetMesh()->GetComponentLocation());
				}
				OldTick = 0;
			}
		}
		else
		{
			OldTick = 0;
		}
	}

	RotateBodyTowardsMovementDirection();
}

void ABaseTank::RotateToCursor(const FVector& LookAtTarget) const
{
	const FRotator FindLookAtRotation = UKismetMathLibrary::FindLookAtRotation(TurretMeshComponent->GetComponentLocation(), LookAtTarget);
	const FVector ToTarget = LookAtTarget - TurretMeshComponent->GetComponentLocation();
	const FRotator LookAtRotation = FRotator(0.0f, FindLookAtRotation.Yaw + 90.0f, 0.0f);

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
			10.0f
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

		if (FireEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				this,
				FireEffect,
				ProjectileSpawnPoint->GetComponentLocation(),
				ProjectileSpawnPoint->GetComponentRotation()
			);
		}

		if (FireSound != nullptr) {
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, TurretMeshComponent->GetComponentLocation());
		}
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

UToynkGameInstance* ABaseTank::GetToynkGameInstance()
{
	return ToynkGameInstance;
}

AToynkGameMode* ABaseTank::GetToynkGameMode()
{
	return ToynkGameMode;
}
