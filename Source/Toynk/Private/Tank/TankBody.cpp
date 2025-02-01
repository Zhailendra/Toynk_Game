#include "Tank/TankBody.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"

ATankBody::ATankBody()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	Tags.Add(FName("controllableActor"));
}

void ATankBody::BeginPlay()
{
	Super::BeginPlay();

	if (const auto TankPlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(TankPlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerContext, 0);
		}
	}

	if (PC)
	{
		PC->bShowMouseCursor = true;
		PC->bEnableClickEvents = true;
		PC->bEnableMouseOverEvents = true;
		PC->CurrentMouseCursor = EMouseCursor::Crosshairs;
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