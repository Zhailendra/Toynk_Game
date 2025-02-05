#include "Tank/TankBody.h"

#include "EngineUtils.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Level/Base_Level.h"

ATankBody::ATankBody()
{
	Tags.Add(FName("controllableActor"));
}

void ATankBody::BeginPlay()
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

void ATankBody::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATankBody::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATankBody::Move);
		}
		if (FireAction)
		{
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ATankBody::Fire);
		}
		if (DropLandMineAction)
		{
			EnhancedInputComponent->BindAction(DropLandMineAction, ETriggerEvent::Started, this, &ATankBody::DropLandMine);
		}
	}
}

void ATankBody::Move(const FInputActionValue& Value)
{
	const FVector2D MoveDirection = Value.Get<FVector2d>();

	if (Controller)
	{
		FVector DeltaLocation = FVector::ZeroVector;
		DeltaLocation.X = MoveDirection.Y * MoveSpeed * UGameplayStatics::GetWorldDeltaSeconds(this);
		AddActorLocalOffset(DeltaLocation, true);

		FRotator DeltaRotation = FRotator::ZeroRotator;
		DeltaRotation.Yaw = MoveDirection.X * RotateSpeed * UGameplayStatics::GetWorldDeltaSeconds(this);
		AddActorLocalRotation(DeltaRotation, true);
	}
}
