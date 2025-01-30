// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet/Bullet.h"
#include "Components/BoxComponent.h"

ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;

	BaseMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMeshComponent"));
	BaseMeshComponent->SetupAttachment(BoxComponent);
}

void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Start = BoxComponent->GetComponentLocation();
	FVector End = Start + (GetActorForwardVector() * Step);

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult, Start, End, ECC_Visibility, CollisionParams
	);

	if (bHit)
	{
		if (Bounce < MaxBounce) {
			Bounce++;

			FVector ImpactNormal = HitResult.Normal;
			FVector NewDirection = GetActorForwardVector() - 2 * FVector::DotProduct(GetActorForwardVector(), ImpactNormal) * ImpactNormal;

			NewDirection.Normalize();
			SetActorRotation(NewDirection.Rotation());
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *HitResult.GetActor()->GetName());
		}
	}
	else {
		SetActorLocation(End);
	}

	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 2.0f);
}
