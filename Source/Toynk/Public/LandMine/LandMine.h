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

	protected:
		virtual void BeginPlay() override;

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

		/*UPROPERTY(EditAnywhere, Category = "Properties")
		UNiagaraSystem* ExplosionEffect;

		UPROPERTY(EditAnywhere, Category = "Properties")
		USoundCue* ExplosionSound;*/

		UPROPERTY(EditAnywhere, Category = "Properties")
		float ExplosionDamage = 100.0f;

		UPROPERTY(EditAnywhere, Category = "Properties")
		bool bIsArmed;

};
