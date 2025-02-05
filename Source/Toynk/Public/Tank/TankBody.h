#pragma once

#include "CoreMinimal.h"
#include "Common/BasePawn.h"
#include "InputActionValue.h"
#include "TankBody.generated.h"

class UInputMappingContext;
class UInputAction;

/**
 * 
 */

UCLASS()
class TOYNK_API ATankBody : public ABasePawn
{
	GENERATED_BODY()

	public:

		ATankBody();

		virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
		virtual void Tick(float DeltaTime) override;
		
	protected:
		virtual void BeginPlay() override;

		UPROPERTY(EditAnywhere, Category = "Tank Input")
		UInputMappingContext* PlayerContext;

		UPROPERTY(EditAnywhere, Category = "Tank Input")
		UInputAction* MoveAction;

		UPROPERTY(EditAnywhere, Category = "Tank Input")
		UInputAction* DropLandMineAction;

		UPROPERTY(EditAnywhere, Category = "Tank Input")
		UInputAction* FireAction;

		void Move(const FInputActionValue &Value);
		
	private:

		UPROPERTY(EditAnywhere, Category = "Tank Movement", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float MoveSpeed = 400.0f;
			
		UPROPERTY(EditAnywhere, Category = "Tank Movement", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float RotateSpeed = 45.0f;

};
