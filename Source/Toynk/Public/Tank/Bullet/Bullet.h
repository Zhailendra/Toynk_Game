#pragma once

#include "CoreMinimal.h"
#include "ObjectPoolIng/Poolable.h"
#include "Bullet.generated.h"

/**
 * 
 */

class UNiagaraComponent;
class UPoolSubsystem;
class UBoxComponent;
class UHealthComponent;
class UNiagaraSystem;
class USoundCue;

UCLASS()
class TOYNK_API ABullet : public AActor, public IPoolable
{
	GENERATED_BODY()

	public:
		ABullet();

		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;

		void InitBullet(APawn* Pawn, float _bulletSpeed, int _maxBounce);

		void SetBulletSpeed(int _speed);
		void SetBulletMaxBounce(int _bounce);

		void ReturnToPool();
	
		virtual void OnSpawnFromPool_Implementation() override;
		virtual void OnReturnToPool_Implementation() override;

	protected:

		void StartEffect() const;
		void PlaySound() const;

		UFUNCTION()
		void OnBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
		UPROPERTY(EditAnywhere, Category = "Bullet Properties", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float BulletSpeed = 1500.0f;

		UPROPERTY(EditAnywhere, Category = "Bullet Properties", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float Damage = 50.f;

		UPROPERTY(EditAnywhere, Category = "Bullet Properties", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int MaxBounce = 1;

		float RealBulletSpeed = 1500.0f;
		int RealMaxBounce = 1;

		int Bounce = 0;

		UPROPERTY(EditAnywhere, Category = "Bullet Properties", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool bIsActive = true;

		UPROPERTY()
		UPoolSubsystem* PoolSubsystem;


	private:
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* BoxComponent;
	
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* BaseMeshComponent;

		UPROPERTY(EditAnywhere, Category = "Niagra")
		UNiagaraSystem* ExplosionEffect;

		UPROPERTY(EditAnywhere, Category = "Niagra")
		UNiagaraSystem* RicochetEffect;

		UPROPERTY(EditAnywhere, Category = "Sound Properties")
		USoundCue* RicochetSound;

		UPROPERTY(EditAnywhere, Category = "Sound Properties")
		USoundCue* WallHitSound;

		UPROPERTY(EditAnywhere, Category = "Sound Properties")
		USoundCue* EnemyHitSound;
	
		FVector InitialPosition;
		float MaxRange = 500.0f;
		
};
