#include "Tank/Bullet/Bullet.h"

#include "Components/BoxComponent.h"
#include "Components/HealthComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "NiagaraFunctionLibrary.h"
#include "ObjectPoolIng/PoolSubsystem.h"

ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add(FName("ProjectileActor"));

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;
	
	BaseMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMeshComponent"));
	BaseMeshComponent->SetupAttachment(BoxComponent);
	BaseMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	InitialPosition = GetActorLocation();
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();

	PoolSubsystem = GetWorld()->GetSubsystem<UPoolSubsystem>();
}

void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsActive) return;

	FVector Start = BoxComponent->GetComponentLocation();
	FVector End = Start + (GetActorForwardVector() * BulletSpeed * DeltaTime);

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams);

	if (bHit)
	{
		AActor* OtherActor = HitResult.GetActor();

		UHealthComponent* HealthComp = (OtherActor) ? OtherActor->FindComponentByClass<UHealthComponent>() : nullptr;

		if (Bounce < MaxBounce && !HealthComp) 
		{
			Bounce++;

			if (RicochetEffect)
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(
					this,
					RicochetEffect,
					HitResult.ImpactPoint,
					FRotationMatrix::MakeFromZ(HitResult.ImpactNormal).Rotator()
				);
			}

			if (RicochetSound != nullptr) {
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), RicochetSound, GetActorLocation());
			}

			FVector ImpactNormal = HitResult.Normal;
			FVector NewDirection = GetActorForwardVector() - 2 * FVector::DotProduct(GetActorForwardVector(), ImpactNormal) * ImpactNormal;
			NewDirection.Normalize();

			SetActorRotation(NewDirection.Rotation());
		}
		else 
		{
			if (OtherActor /*&& OtherActor != GetOwner()*/ && HealthComp)
			{
				AController* MyOwnerInstigator = GetOwner() ? GetOwner()->GetInstigatorController() : nullptr;
				if (MyOwnerInstigator)
				{
					UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwnerInstigator, this, UDamageType::StaticClass());

					if (EnemyHitSound != nullptr) {
						UGameplayStatics::PlaySoundAtLocation(GetWorld(), EnemyHitSound, GetActorLocation());
					}
				}
			}
			else {
				if (WallHitSound != nullptr) {
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), WallHitSound, GetActorLocation());
				}
			}

			if (ExplosionEffect)
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(
					this,
					ExplosionEffect,
					GetActorLocation(),
					GetActorRotation()
				);
			}

			if (PoolSubsystem)
			{
				Bounce = 0;
				PoolSubsystem->ReturnToPool(this);
			}
		}
	}
	else 
	{
		SetActorLocation(End);
	}

	//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 2.0f);
}

void ABullet::InitBullet(APawn* Pawn)
{
	SetOwner(Pawn);
}

void ABullet::ReturnToPool()
{
	PoolSubsystem->ReturnToPool(this);
}


void ABullet::OnSpawnFromPool_Implementation()
{
	SetActorHiddenInGame(false);
	BoxComponent->SetSimulatePhysics(true);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	bIsActive = true;
}

void ABullet::OnReturnToPool_Implementation()
{
	SetOwner(nullptr);
	SetActorHiddenInGame(true);
	BoxComponent->SetSimulatePhysics(false);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bIsActive = false;
}

