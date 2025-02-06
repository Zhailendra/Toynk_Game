// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Bullet.generated.h"

/**
 * 
 */

class UBoxComponent;
class UHealthComponent;

UCLASS()
class TOYNK_API ABullet : public AActor
{
	GENERATED_BODY()

	public:
		ABullet();

		virtual void Tick(float DeltaTime) override;

	protected:
		void DestroyProjectile();

	private:
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* BaseMeshComponent;

		UPROPERTY(EditAnywhere, Category = "Bullet Properties", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float BulletSpeed = 2000.0f;

		UPROPERTY(EditAnywhere, Category = "Bullet Properties", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float Damage = 50.f;

		UPROPERTY(EditAnywhere, Category = "Bullet Properties", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int MaxBounce = 1;

		int Bounce = 0;

		FTimerHandle DelayBeforeDestroy;

		UPROPERTY(EditAnywhere, Category = "Bullet Properties")
		float DelayDestroy = 2.f;
};
