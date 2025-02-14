#include "Enemy/MovableEnemy.h"
#include "Tank/PlayerTank.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "GameFramework/Controller.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"

AMovableEnemy::AMovableEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
	MovementComponent->SetUpdatedComponent(RootComponent);
	MovementComponent->MaxSpeed = EnemySpeed;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	PawnSensingComponent->SetPeripheralVisionAngle(80.f);
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AMovableEnemy::OnPlayerSeen);
}

void AMovableEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	bIsPlayerSeen = false;
	bIsNotFollowingPlayer = false;
	EnemyAIController = Cast<AAIController>(GetController());

	PlayerTank = Cast<APlayerTank>(GetWorld()->GetFirstPlayerController()->GetPawn());

	RandomRoam();
}

void AMovableEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bIsPlayerSeen)
	{
		CheckIfPlayerStillVisible();
	}
	else if (!EnemyAIController->IsFollowingAPath()) 
	{
		RandomRoam();
	}
}

void AMovableEnemy::RandomRoam()
{
	if (!EnemyAIController) return;

	FVector Origin = GetActorLocation();
	FNavLocation RandomLocation;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (NavSystem)
	{
		int Attempts = 5;
		while (Attempts-- > 0)
		{
			if (NavSystem->GetRandomReachablePointInRadius(Origin, 1000.f, RandomLocation))
			{
				FVector Destination = RandomLocation.Location;
				
				FRotator TargetRotation = (Destination - GetActorLocation()).Rotation();
				SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRotation, GetWorld()->GetDeltaSeconds(), 10.0f));

				FAIMoveRequest MoveRequest;
				MoveRequest.SetGoalLocation(Destination);
				MoveRequest.SetAcceptanceRadius(5.0f);

				FNavPathSharedPtr NavPath;
				EnemyAIController->MoveTo(MoveRequest, &NavPath);
				return;
			}
		}
	}
}

void AMovableEnemy::OnPlayerSeen(APawn* SeenPawn)
{
	if (!PlayerTank || SeenPawn != PlayerTank) return;

	if (HasLineOfSightToPlayer())
	{
		bIsPlayerSeen = true;

		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(PlayerTank);
		MoveRequest.SetAcceptanceRadius(DistanceFromPlayer);

		FNavPathSharedPtr NavPath;
		EnemyAIController->MoveTo(MoveRequest, &NavPath);

		GetWorld()->GetTimerManager().ClearTimer(LossOfSightTimer);
	}
}

bool AMovableEnemy::HasLineOfSightToPlayer() const
{
	if (!PlayerTank) return false;

	FVector StartLocation = GetActorLocation();
	FVector EndLocation = PlayerTank->GetActorLocation();

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECC_Visibility,
		CollisionParams
	);

	return !bHit || HitResult.GetActor() == PlayerTank;
}

void AMovableEnemy::CheckIfPlayerStillVisible()
{
	if (!HasLineOfSightToPlayer())
	{
		if (!GetWorld()->GetTimerManager().IsTimerActive(LossOfSightTimer))
		{
			GetWorld()->GetTimerManager().SetTimer(
				LossOfSightTimer,
				this,
				&AMovableEnemy::LoseSightOfPlayer,
				.5f,
				false
			);
		}
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(LossOfSightTimer);
	}
}

void AMovableEnemy::LoseSightOfPlayer()
{
	bIsPlayerSeen = false;
	RandomRoam();
}
