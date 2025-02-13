#include "Tank/LandMine/LandMine.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"

#include "Kismet/GameplayStatics.h"
#include "ObjectPoolIng/PoolSubsystem.h"
#include "Sound/SoundCue.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/BlockingVolume.h"
#include "Level/Walls/BaseWall.h"
#include "Tank/Bullet/Bullet.h"

ALandMine::ALandMine()
{
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add(FName("LandMine"));

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(GetRootComponent());

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(BoxComponent);

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetupAttachment(MeshComponent);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(MeshComponent);

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SceneComponent->SetupAttachment(MeshComponent);
	ExplosionSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ExplosionSpawnPoint"));
	ExplosionSpawnPoint->SetupAttachment(SceneComponent);

	bIsArmed = false;
	bIsExploding = false;

	if (MeshComponent)
	{
		const FVector NewScale(0.3f, 0.3f, 0.3f);
		MeshComponent->SetWorldScale3D(NewScale);
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	if (BoxComponent)
	{
		BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ALandMine::OnBoxOverlapBegin);
		BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ALandMine::OnBoxOverlapEnd);
	}
	if (CapsuleComponent)
	{
		CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ALandMine::OnCapsuleOverlapBegin);
	}
}

void ALandMine::BeginPlay()
{
	Super::BeginPlay();

	PoolSubsystem = GetWorld()->GetSubsystem<UPoolSubsystem>();
	LifeTime = 10.0f;
}

void ALandMine::OnBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("DestroyableActor") && bIsArmed)
	{
		ApplyDamageTo(OtherActor);
		Explode();
	}
}

void ALandMine::OnBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->ActorHasTag("DestroyableActor"))
	{
		if (OtherActor == GetOwner())
		{
			GetWorld()->GetTimerManager().SetTimer(TimerWaitForArmed, this, &ALandMine::ArmBomb, .5f, false);
			SetActorHiddenInGame(false);
		}
	}
}

void ALandMine::OnCapsuleOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("ProjectileActor") && bIsArmed)
	{
		ABullet* Bullet = Cast<ABullet>(OtherActor);

		if (Bullet)
		{
			Bullet->ReturnToPool();
		}
		Explode();
	}
}

void ALandMine::InitLandMine(APawn* Pawn)
{
	SetOwner(Pawn);

	bIsArmed = false;
	bIsExploding = false;
}

void ALandMine::ApplyDamageTo(AActor* Actor)
{
	auto DamageTypeClass = UDamageType::StaticClass();
	auto MyOwnerInstigator = GetOwner() ? GetOwner()->GetInstigatorController() : nullptr;
	UGameplayStatics::ApplyDamage(Actor, ExplosionDamage, MyOwnerInstigator, this, DamageTypeClass);
}

void ALandMine::ReturnToPool()
{
	if (!IsHidden()) {
		if (ExplosionSound != nullptr) {
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
		}

		GetWorld()->GetTimerManager().ClearTimer(TickSoundTimerHandle);

		if (ExplosionEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				this,
				ExplosionEffect,
				ExplosionSpawnPoint->GetComponentLocation(),
				ExplosionSpawnPoint->GetComponentRotation()
			);
		}
	}

	if (PoolSubsystem)
	{
		PoolSubsystem->ReturnToPool(this);
	}
}

void ALandMine::StartTimer()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_LifeTime, this, &ALandMine::Explode, LifeTime, false);
	PlayTickSound();
}

void ALandMine::Explode()
{
	bIsExploding = true;

	TArray<AActor* > OverlappingActors;

	SphereComponent->GetOverlappingActors(OverlappingActors);

	for (AActor* OverlappingActor : OverlappingActors)
	{
		if (ABaseWall* Wall = Cast<ABaseWall>(OverlappingActor))
		{
			Wall->SpawnCoins();
		}
		
		ApplyDamageTo(OverlappingActor);
	}
	
	ReturnToPool();
}

void ALandMine::PlayTickSound()
{
	if (TickTickSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, TickTickSound, GetActorLocation());
	}

	LifeTimeRemaining -= GetWorld()->GetTimerManager().GetTimerElapsed(TickSoundTimerHandle);

	float Progress = LifeTimeRemaining / LifeTime;
	float NewInterval = MinTickInterval + (InitialTickInterval - MinTickInterval) * FMath::Pow(Progress, 2);

	if (LifeTimeRemaining > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(TickSoundTimerHandle, this, &ALandMine::PlayTickSound, NewInterval, false);
	}
}

void ALandMine::ArmBomb()
{
	bIsArmed = true;
	StartTimer();
}

void ALandMine::OnSpawnFromPool_Implementation()
{
	SetActorHiddenInGame(true);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	bIsArmed = false;
	LifeTimeRemaining = LifeTime;
	
	if (DeploySound != nullptr) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeploySound, GetActorLocation(), 0.2);
	}
}

void ALandMine::OnReturnToPool_Implementation()
{
	SetOwner(nullptr);
	SetActorHiddenInGame(true);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bIsArmed = false;
}
