// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CapstoneProjectCharacter.h"
#include "HealerCharacter.generated.h"

class USphereComponent;
class UParticleSystemComponent;
class UNiagaraComponent;
class AActor;

UCLASS()
class CAPSTONEPROJECT_API AHealerCharacter : public ACapstoneProjectCharacter
{
	GENERATED_BODY()
	
public:
	AHealerCharacter();
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Healer")
	TObjectPtr<USphereComponent> HealingZone;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Healer")
	float HealingRadius = 300.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Healer")
	float HealInterval = 3.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Healer")
	float HealAmount = 10.f;
	
	FTimerHandle HealingTimerHandle;
	
	void HealAlliesInZone();
};
