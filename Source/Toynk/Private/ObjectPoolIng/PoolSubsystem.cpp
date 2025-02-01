#include "ObjectPoolIng/PoolSubsystem.h"
#include "ObjectPoolIng/Poolable.h"

void UPoolSubsystem::SpawnFromPool(TSubclassOf<AActor> PoolClass, FVector Location, FRotator Rotation,
	AActor*& SpawnedActor, APawn* Pawn)
{
	SpawnedActor = SpawnFromPool<AActor>(PoolClass, Location, Rotation, Pawn);
}

void UPoolSubsystem::ReturnToPool(AActor* PoolableActor)
{
	const UClass* PoolableClass = PoolableActor->GetClass();
	
	if (PoolableClass->ImplementsInterface(UPoolable::StaticClass()))
	{
		IPoolable::Execute_OnReturnToPool(PoolableActor);
		FPoolArray* ObjectPool = ObjectPools.Find(PoolableClass);
		ObjectPool->Add(PoolableActor);
	} else
	{
		PoolableActor->Destroy();
	}
}
