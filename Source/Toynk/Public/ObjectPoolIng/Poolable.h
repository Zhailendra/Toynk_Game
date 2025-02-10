#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Poolable.generated.h"

UINTERFACE(MinimalAPI)
class UPoolable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TOYNK_API IPoolable
{
	GENERATED_BODY()

	public:

		UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Poolable")
		void OnSpawnFromPool();

		UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Poolable")
		void OnReturnToPool();
};
