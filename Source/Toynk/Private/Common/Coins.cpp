#include "Common/Coins.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ObjectPoolIng/PoolSubsystem.h"
#include "Tank/PlayerTank.h"

ACoins::ACoins()
{
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add(FName("Coins"));

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SceneComponent->SetupAttachment(RootComponent);
		
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(SceneComponent);
	BoxComponent->SetEnableGravity(false);
	
	BaseMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMeshComponent"));
	BaseMeshComponent->SetupAttachment(BoxComponent);
	BaseMeshComponent->SetRelativeScale3D(FVector(4.f, 4.f, 4.f));
	BaseMeshComponent->SetEnableGravity(false);

	if (BoxComponent)
	{
		BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ACoins::OnBoxOverlapBegin);
	}
}

void ACoins::BeginPlay()
{
	Super::BeginPlay();

	PoolSubsystem = GetWorld()->GetSubsystem<UPoolSubsystem>();
	PlayerTank = Cast<APlayerTank>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void ACoins::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float RotationSpeed = 100.f; 

	FRotator NewRotation = FRotator(0.f, RotationSpeed * DeltaTime, 0.f);

	BaseMeshComponent->AddLocalRotation(NewRotation);
}

void ACoins::OnBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor == PlayerTank)
	{
		PlayerTank->AddCoins(10);
		ReturnToPool();
	}
}

void ACoins::ReturnToPool()
{
	PoolSubsystem->ReturnToPool(this);
}

void ACoins::OnSpawnFromPool_Implementation()
{
	SetActorHiddenInGame(false);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ACoins::OnReturnToPool_Implementation()
{
	SetActorHiddenInGame(true);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


