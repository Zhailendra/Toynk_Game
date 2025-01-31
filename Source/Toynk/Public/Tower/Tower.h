#pragma once

#include "CoreMinimal.h"
#include "Common/BasePawn.h"
#include "Tower.generated.h"

class ATankBody;

/**
 *
 */

UCLASS()
class TOYNK_API ATower : public ABasePawn
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

private:
	ATankBody* PlayerTank;

	UPROPERTY(EditAnywhere, Category = "Canon Properties")
	float FireRange = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Canon Properties")
	float FireRate = 2.f;

	FTimerHandle FireRateTimerHandle;

	void CheckFireCondition();

	bool InFireRange();
};