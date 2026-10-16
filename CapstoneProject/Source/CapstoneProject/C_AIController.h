// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionTypes.h"
#include "C_AIController.generated.h"

class UAISenseConfig_Sight;

/**
 * 
 */
UCLASS()
class CAPSTONEPROJECT_API AC_AIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AC_AIController();
	
	virtual void OnPossess(class APawn* InPawn) override;
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;
	
	UFUNCTION()
	void OnTargetSensed(AActor* Actor, FAIStimulus Stimulus);
	
	UPROPERTY(VisibleAnywhere, Category = "AI")
	UAISenseConfig_Sight* SightConfig;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName TargetActorKey = TEXT("TargetActor");
};
