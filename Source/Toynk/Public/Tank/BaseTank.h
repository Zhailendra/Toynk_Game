#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Common/ToynkGameInstance.h"
#include "BaseTank.generated.h"

class UPoolSubsystem;

class UCapsuleComponent;
class UBoxComponent;
class UArrowComponent;
class UNiagaraSystem;
class USoundCue;

class AToynkGameMode;
class ABullet;
class ALandMine;

UCLASS()
class TOYNK_API ABaseTank : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseTank();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void RotateToCursor(const FVector& LookAtTarget) const;
	void RotateBodyTowardsMovementDirection() const;

	void Fire();
	void ResetFire();

	void DropLandMine();
	void ResetDropMine();

	void ManageChainSound(const float DeltaTime);

	UToynkGameInstance* GetToynkGameInstance();
	AToynkGameMode* GetToynkGameMode();

protected:
	UPROPERTY()
	UToynkGameInstance* ToynkGameInstance;

	UPROPERTY()
	AToynkGameMode* ToynkGameMode;

	UPROPERTY()
	APlayerController* PC;

	UPROPERTY()
	UPoolSubsystem* PoolSubsystem;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	TSubclassOf<ABullet> BulletClass;
		
	UPROPERTY(EditAnywhere, Category = "LandMine")
	TSubclassOf<ALandMine> LandMineClass;

	UPROPERTY(EditAnywhere, Category = "Component", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMeshComponent;

	UPROPERTY(EditAnywhere, Category = "Component", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TurretMeshComponent;

	UPROPERTY(EditAnywhere, Category = "Component", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UArrowComponent* ForwardArrowComponent;

	UPROPERTY(EditAnywhere, Category = "Component", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UArrowComponent* RightArrowComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	USceneComponent* LandMineSpawnPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	UActorComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComponent;

	FTimerHandle TimerHandle_DropMineCooldown;
	FTimerHandle TimerHandle_FireCooldown;

	UPROPERTY(EditAnywhere, Category = "Cooldowns")
	float FireCooldown = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Cooldowns")
	float DropMineCooldown = 3.0f;

private:

	UPROPERTY(EditAnywhere, Category = "Common Properties")
	float InterpSpeed = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Tank Properties")
	int MaxLandMines = 5;

	UPROPERTY(EditAnywhere, Category = "Tank Properties")
	int CurrentLandMines = 0;

	float OldTick;

	UPROPERTY(EditAnywhere, Category = "Properties")
	UNiagaraSystem* FireEffect;

	UPROPERTY(EditAnywhere, Category = "Sound Properties")
	USoundCue* ChainSound;

	UPROPERTY(EditAnywhere, Category = "Sound Properties")
	USoundCue* FireSound;

	bool bCanFire = true;
	bool bCanDropMine = true;
};
