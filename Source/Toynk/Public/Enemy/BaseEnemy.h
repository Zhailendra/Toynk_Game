#pragma once

#include "CoreMinimal.h"
#include "Tank/BaseTank.h"
#include "BaseEnemy.generated.h"

class APlayerTank;

/**
 *
 */

UCLASS()
class TOYNK_API ABaseEnemy : public ABaseTank
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	APlayerTank* PlayerTank;

	UPROPERTY(EditAnywhere, Category = "Canon Properties")
	float FireRange = 5000.f;

	UPROPERTY(EditAnywhere, Category = "Canon Properties")
	float FireRate = 2.f;

	FTimerHandle FireRateTimerHandle;

	void CheckFireCondition();

	bool InFireRange();
};