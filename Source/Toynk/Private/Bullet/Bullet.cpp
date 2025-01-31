// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet/Bullet.h"
#include "Components/BoxComponent.h"
#include "Components/HealthComponent.h"
#include "Kismet/GameplayStatics.h"

ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;

	BaseMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMeshComponent"));

	RootComponent = BaseMeshComponent;
}

void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Start = BaseMeshComponent->GetComponentLocation();
	FVector End = Start + (GetActorForwardVector() * BulletSpeed * DeltaTime);

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult, Start, End, ECC_Visibility, CollisionParams
	);

	if (bHit)
	{
		AActor* OtherActor = HitResult.GetActor();

		if (Bounce < MaxBounce && (OtherActor == nullptr || OtherActor->GetOwner() == nullptr || Cast<UHealthComponent>(OtherActor->GetOwner()->FindComponentByClass(UHealthComponent::StaticClass())))) {
			Bounce++;

			FVector ImpactNormal = HitResult.Normal;
			FVector NewDirection = GetActorForwardVector() - 2 * FVector::DotProduct(GetActorForwardVector(), ImpactNormal) * ImpactNormal;

			NewDirection.Normalize();
			SetActorRotation(NewDirection.Rotation());
		}
		else {
			if (OtherActor && OtherActor != GetOwner()) {
				auto DamageTypeClass = UDamageType::StaticClass();
				auto MyOwnerInstigator = GetOwner()->GetInstigatorController();
				UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwnerInstigator, this, DamageTypeClass);
			}

			Destroy();
		}
	}
	else {
		SetActorLocation(End);
	}

	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 2.0f);
}
