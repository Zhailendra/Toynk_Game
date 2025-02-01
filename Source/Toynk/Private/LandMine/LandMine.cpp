#include "LandMine/LandMine.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

ALandMine::ALandMine()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(BoxComponent);


	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SceneComponent->SetupAttachment(MeshComponent);
	ExplosionSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ExplosionSpawnPoint"));
	ExplosionSpawnPoint->SetupAttachment(SceneComponent);

	if (MeshComponent)
	{
		const FVector NewScale(0.3f, 0.3f, 0.3f);
		MeshComponent->SetWorldScale3D(NewScale);
	}
	if (BoxComponent)
	{
		BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ALandMine::OnOverlapBegin);
	}
}

void ALandMine::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALandMine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALandMine::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ALandMine::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("controllableActor"))
	{
		if (OtherActor && OtherActor != GetOwner()) {
			auto DamageTypeClass = UDamageType::StaticClass();
			auto MyOwnerInstigator = GetOwner()->GetInstigatorController();
			UGameplayStatics::ApplyDamage(OtherActor, ExplosionDamage, MyOwnerInstigator, this, DamageTypeClass);
		}
		Destroy();
	}
}

