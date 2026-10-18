// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CAPSTONEPROJECT_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();
	
	UPROPERTY(ReplicatedUsing=OnRep_Health, VisibleAnywhere, BlueprintReadOnly, Category="Health")
	float Health=100.f;
	
	UFUNCTION()
	void OnRep_Health(float OldHealth);
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Health")
	float MaxHealth = 100.f;
	
	UFUNCTION(BlueprintCallable, Category="Health")
	void ApplyDamage(float Damage);
	
	UFUNCTION(BlueprintPure, Category="Health")
	float GetHealthPercent() const;
	
	UFUNCTION(BlueprintPure, Category="Health")
	float GetMaxHealth() const;
	
	UFUNCTION(BlueprintPure, Category="Health")
	float GetCurrentHealth() const;
	
	UFUNCTION(BlueprintPure, Category="Health")
	bool IsDead() const;
	
	UFUNCTION(BlueprintCallable, Category="Health")
	void Heal(float Amount);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
