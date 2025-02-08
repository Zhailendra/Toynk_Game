#include "Tank/LandMine/LandMine.h"

#include "Components/BoxComponent.h"

#include "Kismet/GameplayStatics.h"
#include "ObjectPoolIng/PoolSubsystem.h"

ALandMine::ALandMine()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(GetRootComponent());

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(BoxComponent);

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SceneComponent->SetupAttachment(MeshComponent);
	ExplosionSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ExplosionSpawnPoint"));
	ExplosionSpawnPoint->SetupAttachment(SceneComponent);

	bIsArmed = false;

	if (MeshComponent)
	{
		const FVector NewScale(0.3f, 0.3f, 0.3f);
		MeshComponent->SetWorldScale3D(NewScale);
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	if (BoxComponent)
	{
		BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ALandMine::OnOverlapBegin);
		BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ALandMine::OnOverlapEnd);
	}
}

void ALandMine::BeginPlay()
{
	Super::BeginPlay();

	PoolSubsystem = GetWorld()->GetSubsystem<UPoolSubsystem>();
	LifeTime = 10.0f;
}

void ALandMine::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor && OtherActor->ActorHasTag("DestroyableActor"))
	{
		if (bIsArmed)
		{
			auto DamageTypeClass = UDamageType::StaticClass();
			auto MyOwnerInstigator = GetOwner() ? GetOwner()->GetInstigatorController() : nullptr;
			UGameplayStatics::ApplyDamage(OtherActor, ExplosionDamage, MyOwnerInstigator, this, DamageTypeClass);
			
			ReturnToPool();
		}
	}
}

void ALandMine::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
 	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
 {
	if (OtherActor && OtherActor->ActorHasTag("DestroyableActor"))
	{
		if (OtherActor == GetOwner())
		{
			bIsArmed = true;
			SetActorHiddenInGame(false);
		}
	}
 }

void ALandMine::InitLandMine(APawn* Pawn)
{
	SetOwner(Pawn);
	bIsArmed = false;
}

void ALandMine::ReturnToPool()
{
	if (PoolSubsystem)
	{
		PoolSubsystem->ReturnToPool(this);
	}
}

void ALandMine::OnSpawnFromPool_Implementation()
{
	SetActorHiddenInGame(true);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	bIsArmed = false;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_LifeTime, this, &ALandMine::Explode, LifeTime, false);
}

void ALandMine::OnReturnToPool_Implementation()
{
	SetOwner(nullptr);
	SetActorHiddenInGame(true);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bIsArmed = false;
}

void ALandMine::Explode()
{
	ReturnToPool();
}

