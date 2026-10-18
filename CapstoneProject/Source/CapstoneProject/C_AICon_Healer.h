// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "C_AICon_Healer.generated.h"

class UBehaviorTree;
class ACapstoneProjectCharacter;

UCLASS()
class CAPSTONEPROJECT_API AC_AICon_Healer : public AAIController
{
	GENERATED_BODY()
	
public:
	AC_AICon_Healer();
	
	virtual void OnPossess(APawn* InPawn) override;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset;
	
	UPROPERTY(EditDefaultsOnly, Category="AI")
	FName TargetActorKey = TEXT("TargetActor");
	
	UPROPERTY(EditDefaultsOnly, Category="AI")
	float ReTargetInterval = 10.f;
	
	FTimerHandle ReTargetTimerHandle;
	
	void FindLowestHealthAlly();
};
