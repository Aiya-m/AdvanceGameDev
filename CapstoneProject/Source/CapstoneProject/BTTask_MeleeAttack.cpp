// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MeleeAttack.h"
#include "C_AIController.h"
#include "CapstoneProjectCharacter.h"

UBTTask_MeleeAttack::UBTTask_MeleeAttack()
{
	NodeName = TEXT("Melee Attack");
}

EBTNodeResult::Type UBTTask_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AC_AIController* AICon = Cast<AC_AIController>(OwnerComp.GetAIOwner());
	
	if (AICon == nullptr){return EBTNodeResult::Failed;}
	
	ACapstoneProjectCharacter* Me = Cast<ACapstoneProjectCharacter>(AICon->GetPawn());
	
	if (Me == nullptr){return EBTNodeResult::Failed;}
	
	Me->ServerMeleeAttack();
	
	return EBTNodeResult::Succeeded;
}
