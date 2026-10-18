// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ChaseExplode.generated.h"

class AActor;
class ACapstoneProjectCharacter;
class UBehaviorTreeComponent;

UCLASS()
class CAPSTONEPROJECT_API UBTTask_ChaseExplode : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_ChaseExplode();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
protected:
	UPROPERTY(EditAnywhere, Category="Explosion")
	float ExplosionRadius = 250.f;
	
	UPROPERTY(EditAnywhere, Category="Explosion")
	float ExplosionDamage = 30.f;
	
	UPROPERTY(EditAnywhere, Category="Explosion")
	float ExplosionDelay = 0.5f;
	
	UPROPERTY(EditDefaultsOnly, Category="AI")
	FName TargetActorKey = FName("TargetActor");
	
private:
	FTimerHandle ExplosionTimerHandle;
	TWeakObjectPtr<UBehaviorTreeComponent> CachedOwnerComp;
	TWeakObjectPtr<ACapstoneProjectCharacter> CachedAI;
	TWeakObjectPtr<AActor> CachedTarget;
	
	void Explode();
	
};
