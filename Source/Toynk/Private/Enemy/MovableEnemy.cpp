#include "Enemy/MovableEnemy.h"

#include "Tank/PlayerTank.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "GameFramework/Controller.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Perception/PawnSensingComponent.h"

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
	EnemyAIController = Cast<AAIController>(GetController());

	RandomRoam();
}

void AMovableEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AMovableEnemy::RandomRoam() const
{
	if (!EnemyAIController) return;

	FVector Origin = GetActorLocation();
	FNavLocation RandomLocation;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (NavSystem && NavSystem->GetRandomReachablePointInRadius(Origin, 1000.f, RandomLocation)) 
	{
		FVector Destination = RandomLocation.Location;

		EnemyAIController->MoveToLocation(Destination);
	}
}

void AMovableEnemy::OnPlayerSeen(APawn* SeenPawn)
{
	if (SeenPawn == PlayerTank)
	{
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

		if (!bHit || HitResult.GetActor() == PlayerTank)
		{
			bIsPlayerSeen = true;
			EnemyAIController->MoveToActor(PlayerTank, DistanceFromPlayer);
		}
		else
		{
			RandomRoam();
			bIsPlayerSeen = false;
		}
	}
	else
	{
		RandomRoam();
		bIsPlayerSeen = false;
	}
}
