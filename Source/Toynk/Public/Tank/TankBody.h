#pragma once

#include "CoreMinimal.h"
#include "Common/BasePawn.h"
#include "InputActionValue.h"
#include "TankBody.generated.h"

class USpringArmComponent;
class UCameraComponent;
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
		UInputAction* RotateAction;

		UPROPERTY(EditAnywhere, Category = "Tank Input")
		UInputAction* FireAction;

		void Move(const FInputActionValue &Value);
		
	private:
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* SpringArmComponent;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UCameraComponent* CameraComponent;

		UPROPERTY(EditAnywhere, Category = "Tank Movement", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float MoveSpeed = 400.0f;
			
		UPROPERTY(EditAnywhere, Category = "Tank Movement", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float RotateSpeed = 45.0f;
	
};
