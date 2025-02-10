#pragma once

#include "CoreMinimal.h"
#include "BaseTank.h"
#include "PlayerTank.generated.h"

struct FInputActionValue;
class UInputMappingContext;
class UInputAction;

/**
 * 
 */
UCLASS()
class TOYNK_API APlayerTank : public ABaseTank
{
	GENERATED_BODY()

public:
	APlayerTank();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	void Move(const FInputActionValue& Value);
	
private:
	
	UPROPERTY(EditAnywhere, Category = "Player Input")
	UInputMappingContext* PlayerContext;

	UPROPERTY(EditAnywhere, Category = "Player Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Player Input")
	UInputAction* DropLandMineAction;

	UPROPERTY(EditAnywhere, Category = "Player Input")
	UInputAction* FireAction;
	
	FVector CurrentVelocity;

	UPROPERTY(EditAnywhere, Category = "Player Movement", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float MoveSpeed = 200.0f;
};
