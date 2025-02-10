#include "Components/HealthComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "NiagaraFunctionLibrary.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::DamageTaken);

}


void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHealthComponent::DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* Instigator, AActor* DamageCauser)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, ("Damage taken"));

	if (Damage <= 0.f) return;

	CurrentHealth -= Damage;
	if (CurrentHealth <= 0) {
		CurrentHealth = 0;

		if (ExplosionEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				this,
				ExplosionEffect,
				DamagedActor->GetActorLocation(),
				DamagedActor->GetActorRotation()
			);
		}

		if (ExplosionSound != nullptr) {
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, DamagedActor->GetActorLocation());
		}

		GetOwner()->Destroy();
	}
}

