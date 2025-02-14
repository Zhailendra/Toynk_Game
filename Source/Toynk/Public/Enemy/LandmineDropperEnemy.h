#pragma once

#include "CoreMinimal.h"
#include "Enemy/MovableEnemy.h"
#include "LandmineDropperEnemy.generated.h"

/**
 * 
 */
UCLASS()
class TOYNK_API ALandmineDropperEnemy : public AMovableEnemy
{
	GENERATED_BODY()

	public:
		ALandmineDropperEnemy();
	
	protected:
	
		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;

	private:
	
};
