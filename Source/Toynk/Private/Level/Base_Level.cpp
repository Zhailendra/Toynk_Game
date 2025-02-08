#include "Level/Base_Level.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

ABase_Level::ABase_Level()
{
	PrimaryActorTick.bCanEverTick = true;

	BaseMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMeshComponent"));

	BaseFoliageComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseFoliageComponent"));
	BaseFoliageComponent->SetupAttachment(BaseMeshComponent);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(BaseMeshComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void ABase_Level::BeginPlay()
{
	Super::BeginPlay();
	
}

