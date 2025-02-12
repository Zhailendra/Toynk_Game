#include "Level/Walls/BaseWall.h"

#include "Components/BoxComponent.h"
#include "Components/HealthComponent.h"

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

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

void ABaseWall::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

