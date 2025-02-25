#include "Level/Walls/BaseWall.h"

#include "Common/Coins.h"
#include "Components/BoxComponent.h"
#include "Components/HealthComponent.h"
#include "ObjectPoolIng/PoolSubsystem.h"
#include "Common/ToynkGameInstance.h"

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

	ToynkGameInstance = Cast<UToynkGameInstance>(GetGameInstance());

	if (ToynkGameInstance) {
		SetCoinsSpawnChance(ToynkGameInstance->GetCoinRate());
	}

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

void ABaseWall::SetCoinsSpawnChance(float CoinsSpawnChance_)
{
	CoinsSpawnChance = CoinsSpawnChance_;
}

