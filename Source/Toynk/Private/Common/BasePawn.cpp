#include "Common/BasePawn.h"
#include "Bullet/Bullet.h"
#include "Components/BoxComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

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
}

void ABasePawn::BeginPlay()
{
	Super::BeginPlay();

	PC = Cast<APlayerController>(GetController());
}

void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PC)
	{
		FHitResult HitResult;
		PC->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

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
	if (Controller)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("FIRING!"));

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		GetWorld()->SpawnActor<ABullet>(BulletClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation() - FRotator(0, 90, 0), SpawnParams);

		if (FireSFX)
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				FireSFX,
				ProjectileSpawnPoint->GetComponentLocation());
		}

		if (FireVFX)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				this,
				FireVFX,
				ProjectileSpawnPoint->GetComponentLocation(),
				ProjectileSpawnPoint->GetComponentRotation());
		}
	}
}