#include "Level/Walls/BaseWall.h"

#include "Components/BoxComponent.h"

ABaseWall::ABaseWall()
{
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add(FName("DestroyableActor"));

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SceneComponent->SetupAttachment(RootComponent);
		
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(SceneComponent);
	
	BaseMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMeshComponent"));
	BaseMeshComponent->SetupAttachment(BoxComponent);

}

void ABaseWall::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

