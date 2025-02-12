#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWall.generated.h"

class UPoolSubsystem;

class ACoins;
class UBoxComponent;

UCLASS()
class TOYNK_API ABaseWall : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseWall();

	void SpawnCoins() const;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;


private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	USceneComponent* CoinsSpawnPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	UActorComponent* HealthComponent;

	UPROPERTY(EditAnywhere, Category = "Coins")
	TSubclassOf<ACoins> CoinsClass;
	
	UPROPERTY()
	UPoolSubsystem* PoolSubsystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	float CoinsSpawnChance = 0.1f;

};
