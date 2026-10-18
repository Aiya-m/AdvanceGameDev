// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChaseExplode.h"
#include "CapstoneProjectCharacter.h"
#include "HealthComponent.h"
#include "C_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"

UBTTask_ChaseExplode::UBTTask_ChaseExplode()
{
	NodeName = TEXT("SelfExplode");
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_ChaseExplode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AC_AIController* AICon = Cast<AC_AIController>(OwnerComp.GetAIOwner());
	
	if (!AICon) {return EBTNodeResult::Failed;}
	
	ACapstoneProjectCharacter* AI = Cast<ACapstoneProjectCharacter>(AICon->GetPawn());
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	
	//if (!AI||BB){return EBTNodeResult::Failed;}
	
	AActor* Target = Cast<AActor>(BB->GetValueAsObject(TargetActorKey));
	
	if (!IsValid(Target) || Target == AI){return EBTNodeResult::Failed;}
	
	const float Distance = FVector::Dist(AI->GetActorLocation(), Target->GetActorLocation());
	
	if (Distance > ExplosionRadius){return EBTNodeResult::Failed;}
	
	AICon->StopMovement();
	
	CachedOwnerComp = &OwnerComp;
	CachedAI = AI;
	CachedTarget = Target;
	
	GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandle, this, &UBTTask_ChaseExplode::Explode, ExplosionDelay, false);
	return EBTNodeResult::InProgress;
}

void UBTTask_ChaseExplode::Explode()
{
	ACapstoneProjectCharacter* AI = CachedAI.Get();
	
	AActor* Target = CachedTarget.Get();
	
	UBehaviorTreeComponent* OwnerComp = CachedOwnerComp.Get();
	
	if (!AI || !OwnerComp) {return;}
	
	const FVector ExplosionLocation = AI->GetActorLocation();
	
	DrawDebugSphere(GetWorld(), ExplosionLocation, ExplosionRadius, 24, FColor::Red, false, 1.5f, 0, 2.f);
	
	if (IsValid(Target) && FVector::Dist(ExplosionLocation, Target->GetActorLocation()) <= ExplosionRadius)
	{
		if (UHealthComponent* HC = Target->FindComponentByClass<UHealthComponent>())
		{
			HC->ApplyDamage(ExplosionDamage);
		}
	}
	
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
	AI->Destroy();
}