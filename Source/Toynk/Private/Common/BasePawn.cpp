#include "Common/BasePawn.h"
#include "Components/BoxComponent.h"

ABasePawn::ABasePawn()
{
	PrimaryActorTick.bCanEverTick = true;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;
	BaseMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMeshComponent"));
	BaseMeshComponent->SetupAttachment(BoxComponent);
	TurretMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMeshComponent"));
	TurretMeshComponent->SetupAttachment(BaseMeshComponent);
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(TurretMeshComponent);
}

void ABasePawn::BeginPlay()
{
	Super::BeginPlay();

	PC = Cast<APlayerController>(GetController());

	PC->bShowMouseCursor = true;
	PC->bEnableClickEvents = true;
	PC->bEnableMouseOverEvents = true;
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
	FRotator LookAtRotation = FRotator(0.0f, ToTarget.Rotation().Yaw, 0.0f);
	
	TurretMeshComponent->SetWorldRotation(
		FMath::RInterpTo(TurretMeshComponent->GetComponentRotation(), LookAtRotation, GetWorld()->GetDeltaSeconds(), InterpSpeed),
		true
	);
}