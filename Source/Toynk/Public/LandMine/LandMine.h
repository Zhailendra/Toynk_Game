#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "LandMine.generated.h"

class UBoxComponent;
class UNiagaraSystem;
class USoundCue;

UCLASS()
class TOYNK_API ALandMine : public APawn
{
	GENERATED_BODY()

	public:
		ALandMine();

		virtual void Tick(float DeltaTime) override;

		virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	protected:
		virtual void BeginPlay() override;

		UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	private:
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* MeshComponent;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* BoxComponent;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		USceneComponent* SceneComponent;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		USceneComponent* ExplosionSpawnPoint;

		UPROPERTY(EditAnywhere, Category = "Explosion")
		UNiagaraSystem* ExplosionEffect;

		UPROPERTY(EditAnywhere, Category = "Explosion")
		USoundCue* ExplosionSound;

		UPROPERTY(EditAnywhere, Category = "Explosion")
		float ExplosionDamage = 100.0f;

};
