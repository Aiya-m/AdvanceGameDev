// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include"Net/UnrealNetwork.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	SetIsReplicatedByDefault(true);

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner() && GetOwner()->HasAuthority())
	{
		Health = MaxHealth;
	}
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UHealthComponent, Health);
}

void UHealthComponent::OnRep_Health(float OldHealth)
{
	
}

void UHealthComponent::ApplyDamage(float Damage)
{
	if (!GetOwner() || !GetOwner()->HasAuthority()){return; }
	
	if (Damage < 0.f) { return; }
	Health -= Damage;
	if (Health < 0.f) { Health = 0.f; }
}

float UHealthComponent::GetHealthPercent() const
{
	if (MaxHealth <= 0.f) { return 0.f; }
	return Health / MaxHealth;
}

bool UHealthComponent::IsDead() const
{
	return Health <= 0.f;
}

void UHealthComponent::Heal(float Amount)
{
	if (!GetOwner() || !GetOwner()->HasAuthority())
	{
		return;
	}

	if (Amount <= 0.f || IsDead() || MaxHealth <= 0.f)
	{
		return;
	}

	Health = FMath::Clamp(Health + Amount,0.f,MaxHealth);
}

float UHealthComponent::GetMaxHealth() const
{
	return MaxHealth;
}

float UHealthComponent::GetCurrentHealth() const
{
	return Health;
}