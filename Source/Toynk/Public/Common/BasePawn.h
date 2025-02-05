#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"

class UPoolSubsystem;

class UBoxComponent;
class ABullet;
class ALandMine;

UCLASS()
class TOYNK_API ABasePawn : public APawn
{
	GENERATED_BODY()
	
	public:
		ABasePawn();

		virtual void Tick(float DeltaTime) override;

		virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	protected:
		virtual void BeginPlay() override;
		void RotateTurret(const FVector& LookAtTarget) const;

		void ResetFire();
		void ResetDropMine();
	
		void Fire();
		void DropLandMine();

		UPROPERTY(EditAnywhere, Category = "Turret Properties")
		float InterpSpeed = 5.0f;

		UPROPERTY(EditAnywhere, Category = "Pawn Properties")
		int MaxLandMines = 5;

		UPROPERTY(EditAnywhere, Category = "Pawn Properties")
		int CurrentLandMines = 0;

		UPROPERTY()
		APlayerController* PC;

		UPROPERTY()
		UPoolSubsystem* PoolSubsystem;

		UPROPERTY(EditAnywhere, Category = "Bullet")
		TSubclassOf<ABullet> BulletClass;
		
		UPROPERTY(EditAnywhere, Category = "LandMine")
		TSubclassOf<ALandMine> LandMineClass;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		USceneComponent* ProjectileSpawnPoint;
	
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		USceneComponent* LandMineSpawnPoint;

	private:
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* BoxComponent;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* BaseMeshComponent;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		USceneComponent* SceneComponent;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* TurretMeshComponent;

		FTimerHandle TimerHandle_FireCooldown;
		FTimerHandle TimerHandle_DropMineCooldown;

		UPROPERTY(EditAnywhere, Category = "Cooldowns")
		float FireCooldown = 1.0f;

		UPROPERTY(EditAnywhere, Category = "Cooldowns")
		float DropMineCooldown = 3.0f;

		bool bCanFire = true;
		bool bCanDropMine = true;

};
