#pragma once

#include "CoreMinimal.h"
#include "Poolable.h"
#include "Subsystems/WorldSubsystem.h"
#include "PoolSubsystem.generated.h"

USTRUCT()
struct FPoolArray
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<AActor*> ObjectPool;

	bool IsEmpty() const
	{
		return ObjectPool.IsEmpty();
	}

	AActor* Pop()
	{
		return ObjectPool.Pop();
	}

	void Add(AActor* PoolableActor)
	{
		ObjectPool.Add(PoolableActor);
	}
};

UCLASS()
class TOYNK_API UPoolSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

	public:
		UFUNCTION(BlueprintCallable, Category = "PoolSubsystem", meta = (DeterminesOutputType = "PoolClass", DeterminesOutputParam = "SpawnedActor"))
		void SpawnFromPool(TSubclassOf<AActor> PoolClass, FVector Location, FRotator Rotation, AActor*& SpawnedActor, APawn* Pawn);

		template<typename T>
		T* SpawnFromPool(TSubclassOf<T> PoolClass, FVector Location, FRotator Rotation, APawn* Pawn);

		UFUNCTION(BlueprintCallable, Category = "PoolSubsystem")
		void ReturnToPool(AActor* PoolableActor);

		UPROPERTY()
		TMap<UClass*, FPoolArray> ObjectPools;
	
};

template <typename T>
T* UPoolSubsystem::SpawnFromPool(TSubclassOf<T> PoolClass, FVector Location, FRotator Rotation, APawn* Pawn)
{
	T* PooledActor = nullptr;
    
	if (PoolClass.Get()->ImplementsInterface(UPoolable::StaticClass()))
	{
		FPoolArray& ObjectPool = ObjectPools.FindOrAdd(PoolClass);
        
		if (ObjectPool.IsEmpty())
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = Pawn;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			PooledActor = GetWorld()->SpawnActor<T>(PoolClass, Location, Rotation, SpawnParams);
		} else
		{
			PooledActor = CastChecked<T>(ObjectPool.Pop());
			PooledActor->SetActorLocationAndRotation(Location, Rotation);
		}

		IPoolable::Execute_OnSpawnFromPool(PooledActor);
	}

	return PooledActor;
}

