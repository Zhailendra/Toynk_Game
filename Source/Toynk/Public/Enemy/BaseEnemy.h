#pragma once

#include "CoreMinimal.h"
#include "Common/BasePawn.h"
#include "BaseEnemy.generated.h"

class ATankBody;

/**
 *
 */

UCLASS()
class TOYNK_API ABaseEnemy : public ABasePawn
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

	ATankBody* PlayerTank;

	UPROPERTY(EditAnywhere, Category = "Canon Properties")
	float FireRange = 5000.f;

	UPROPERTY(EditAnywhere, Category = "Canon Properties")
	float FireRate = 2.f;

	FTimerHandle FireRateTimerHandle;

	void CheckFireCondition();

	bool InFireRange();
};