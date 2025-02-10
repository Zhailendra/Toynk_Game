#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ObjectPoolIng/Poolable.h"
#include "LandMine.generated.h"

class UPoolSubsystem;
class UNiagaraSystem;

class UBoxComponent;
class USoundCue;

UCLASS()
class TOYNK_API ALandMine : public AActor, public IPoolable
{
	GENERATED_BODY()

	public:
		ALandMine();

		void InitLandMine(APawn* Pawn);

		virtual void OnSpawnFromPool_Implementation() override;
		virtual void OnReturnToPool_Implementation() override;

		void StartTimer();
	
		void Explode();
		void PlayTickSound();

	protected:
		virtual void BeginPlay() override;

		void ReturnToPool();

		UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

		UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

		UPROPERTY()
		UPoolSubsystem* PoolSubsystem;

	private:
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* BoxComponent;
	
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* MeshComponent;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		USceneComponent* SceneComponent;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		USceneComponent* ExplosionSpawnPoint;

		UPROPERTY(EditAnywhere, Category = "Properties")
		UNiagaraSystem* ExplosionEffect;

		UPROPERTY(EditAnywhere, Category = "Sound Properties")
		USoundCue* DeploySound;

		UPROPERTY(EditAnywhere, Category = "Sound Properties")
		USoundCue* ExplosionSound;

		UPROPERTY(EditAnywhere, Category = "Sound Properties")
		USoundCue* TickTickSound;

		FTimerHandle TimerHandle_LifeTime;
		FTimerHandle TickSoundTimerHandle;

		UPROPERTY(EditAnywhere, Category = "Timers")
		float LifeTime = 10.0f;
	
		float InitialTickInterval = 1.0f;
	
		float MinTickInterval = 0.1f;
	
		float LifeTimeRemaining;

		UPROPERTY(EditAnywhere, Category = "Properties")
		float ExplosionDamage = 100.0f;

		UPROPERTY(EditAnywhere, Category = "Properties")
		bool bIsArmed;

};
