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

	if (BoxComponent)
	{
		BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnBoxOverlapBegin);
		BoxComponent->SetEnableGravity(false);
	}

	if (BaseMeshComponent)
	{
		BaseMeshComponent->SetEnableGravity(false);
	}
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
	FVector End = Start + (GetActorForwardVector() * RealBulletSpeed * DeltaTime);

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams);

	if (bHit)
	{
		AActor* OtherActor = HitResult.GetActor();

		UHealthComponent* HealthComp = (OtherActor) ? OtherActor->FindComponentByClass<UHealthComponent>() : nullptr;

		if (Bounce < RealMaxBounce && !HealthComp) 
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
			if (OtherActor && !OtherActor->ActorHasTag("DestroyableWall") && HealthComp)
			{
				AController* MyOwnerInstigator = GetOwner() ? GetOwner()->GetInstigatorController() : nullptr;
				if (MyOwnerInstigator)
				{
					UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwnerInstigator, this, UDamageType::StaticClass());

					PlaySound();
				}
			}
			else {
				if (WallHitSound != nullptr) {
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), WallHitSound, GetActorLocation());
				}
			}

			StartEffect();

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

void ABullet::StartEffect() const
{
	if (ExplosionEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			ExplosionEffect,
			GetActorLocation(),
			GetActorRotation()
		);
	}
}

void ABullet::PlaySound() const
{
	if (EnemyHitSound != nullptr) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), EnemyHitSound, GetActorLocation());
	}
}


void ABullet::OnBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("ProjectileActor"))
	{
		PlaySound();
		StartEffect();
		ReturnToPool();
	}
}

void ABullet::InitBullet(APawn* Pawn, float _bulletSpeed, int _maxBounce)
{
	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	SetOwner(Pawn);

	if (Pawn == PlayerPawn) {
		SetBulletSpeed(_bulletSpeed);
		SetBulletMaxBounce(_maxBounce);
	}
	else {
		SetBulletSpeed(BulletSpeed);
		SetBulletMaxBounce(MaxBounce);
	}
}

void ABullet::SetBulletSpeed(int _speed)
{
	RealBulletSpeed = _speed;
}

void ABullet::SetBulletMaxBounce(int _bounce)
{
	RealMaxBounce = _bounce;
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
