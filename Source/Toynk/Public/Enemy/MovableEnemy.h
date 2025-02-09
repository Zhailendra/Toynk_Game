#pragma once

#include "CoreMinimal.h"
#include "BaseEnemy.h"
#include "GameFramework/Pawn.h"
#include "MovableEnemy.generated.h"

class AAIController;
class UPawnSensingComponent;
class UFloatingPawnMovement;

UCLASS()
class TOYNK_API AMovableEnemy : public ABaseEnemy
{
	GENERATED_BODY()

	public:
		AMovableEnemy();

	protected:
		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;

		UFUNCTION()
		void OnPlayerSeen(APawn* SeenPawn);
	
	private:
		UPROPERTY()
		AAIController* EnemyAIController;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
		UPawnSensingComponent* PawnSensingComponent;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
		UFloatingPawnMovement* MovementComponent;
	
		UPROPERTY(EditAnywhere, Category = "Movement")
		float MoveSpeed = 50.f;

		UPROPERTY(EditAnywhere, Category = "Movement")
		float DistanceFromPlayer = 500.f;

		bool bIsPlayerSeen = false;
};
