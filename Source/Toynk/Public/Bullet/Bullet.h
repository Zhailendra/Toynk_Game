// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Bullet.generated.h"

/**
 * 
 */

class UBoxComponent;

UCLASS()
class TOYNK_API ABullet : public AActor
{
	GENERATED_BODY()

	public:
		ABullet();

		virtual void Tick(float DeltaTime) override;

	protected:
		UPROPERTY(EditAnywhere, Category = "Bullet Properties", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float BulletSpeed = 400.0f;

		UPROPERTY(EditAnywhere, Category = "Bullet Properties", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float Step = 20.0f;

		UPROPERTY(EditAnywhere, Category = "Bullet Properties", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int MaxBounce = 1;

		UPROPERTY(EditAnywhere, Category = "Bullet Properties", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int Bounce = 0;

	private:
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* BoxComponent;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* BaseMeshComponent;
};
