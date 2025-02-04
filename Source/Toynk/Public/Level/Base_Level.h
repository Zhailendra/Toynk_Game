#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Base_Level.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class TOYNK_API ABase_Level : public AActor
{
	GENERATED_BODY()
	
	public:	
		ABase_Level();

	protected:
		virtual void BeginPlay() override;

	private:

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* BaseMeshComponent;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* BaseFoliageComponent;
	
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* SpringArmComponent;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UCameraComponent* CameraComponent;

};
