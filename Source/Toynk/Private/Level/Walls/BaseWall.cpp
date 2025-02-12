#include "Level/Walls/BaseWall.h"

#include "Common/Coins.h"
#include "Components/BoxComponent.h"
#include "Components/HealthComponent.h"
#include "ObjectPoolIng/PoolSubsystem.h"

ABaseWall::ABaseWall()
{
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add(FName("DestroyableWall"));

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SceneComponent->SetupAttachment(RootComponent);
		
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(SceneComponent);
	
	BaseMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMeshComponent"));
	BaseMeshComponent->SetupAttachment(BoxComponent);
	
	CoinsSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("CoinsSpawnPoint"));
	CoinsSpawnPoint->SetupAttachment(BaseMeshComponent);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

void ABaseWall::BeginPlay()
{
	Super::BeginPlay();

	PoolSubsystem = GetWorld()->GetSubsystem<UPoolSubsystem>();
}

void ABaseWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseWall::SpawnCoins() const
{
	if (FMath::FRand() <= CoinsSpawnChance)
	{
		PoolSubsystem->SpawnFromPool<ACoins>(
			CoinsClass,
			CoinsSpawnPoint->GetComponentLocation(),
			CoinsSpawnPoint->GetComponentRotation(),
			nullptr
		);
	}
}

