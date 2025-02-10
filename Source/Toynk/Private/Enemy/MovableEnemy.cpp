#include "Enemy/MovableEnemy.h"

#include "Tank/PlayerTank.h"

#include "AIController.h"
#include "GameFramework/Controller.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Perception/PawnSensingComponent.h"

AMovableEnemy::AMovableEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
	MovementComponent->SetUpdatedComponent(RootComponent);

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	PawnSensingComponent->SetPeripheralVisionAngle(80.f);
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AMovableEnemy::OnPlayerSeen);
}

void AMovableEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	bIsPlayerSeen = false;
	EnemyAIController = Cast<AAIController>(GetController());
}

void AMovableEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMovableEnemy::OnPlayerSeen(APawn* SeenPawn)
{
	if (PlayerTank)
	{
		bIsPlayerSeen = true;
		EnemyAIController->MoveToActor(PlayerTank, DistanceFromPlayer);
	}
}
