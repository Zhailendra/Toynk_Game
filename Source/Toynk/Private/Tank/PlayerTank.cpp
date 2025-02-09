#include "Tank/PlayerTank.h"

#include "Level/Base_Level.h"

#include "EngineUtils.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

class ABase_Level;

APlayerTank::APlayerTank()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APlayerTank::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		for (TActorIterator<ABase_Level> It(GetWorld()); It; ++It)
		{
			ABase_Level* LevelActor = *It;
			if (LevelActor)
			{
				PlayerController->SetViewTargetWithBlend(LevelActor, 1.3f);
				break;
			}
		}
	}

	if (const auto TankPlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(TankPlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerContext, 0);
		}
	}
}

void APlayerTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentVelocity.IsNearlyZero())
	{
		CurrentVelocity = FVector::ZeroVector;
	}
}

void APlayerTank::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerTank::Move);
		}
		if (FireAction)
		{
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &APlayerTank::Fire);
		}
		if (DropLandMineAction)
		{
			EnhancedInputComponent->BindAction(DropLandMineAction, ETriggerEvent::Started, this, &APlayerTank::DropLandMine);
		}
	}
}

void APlayerTank::Move(const FInputActionValue& Value)
{
	const FVector2D MoveDirection = Value.Get<FVector2D>();
	float X = MoveDirection.X;
	float Y = MoveDirection.Y;

	FRotator ControlRotation = GetControlRotation();

	FVector ForwardVector = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::X);
	FVector RightVector = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::Y);

	CurrentVelocity = (ForwardVector * Y + RightVector * X) * MoveSpeed;

	AddMovementInput(ForwardVector, Y);
	AddMovementInput(RightVector, X);
}

