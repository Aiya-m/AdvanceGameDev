// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_EnemyProjectile.generated.h"

/**
 * 
 */
UCLASS()
class CAPSTONEPROJECT_API UBTTask_EnemyProjectile : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_EnemyProjectile();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp,uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category="Combat")
	float AttackRange = 1500.f;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	FName TargetActorKey = TEXT("TargetActor");
};
