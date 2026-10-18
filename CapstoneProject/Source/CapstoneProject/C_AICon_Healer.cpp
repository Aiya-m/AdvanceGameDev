// Fill out your copyright notice in the Description page of Project Settings.


#include "C_AICon_Healer.h"
#include "HealerCharacter.h"
#include "CapstoneProjectCharacter.h"
#include "HealthComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

AC_AICon_Healer::AC_AICon_Healer()
{
	bWantsPlayerState = true;
}

void AC_AICon_Healer::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if (!HasAuthority()){return;}
	
	if (BehaviorTreeAsset){RunBehaviorTree(BehaviorTreeAsset);}
	FindLowestHealthAlly();
	
	GetWorld()->GetTimerManager().SetTimer(ReTargetTimerHandle, this, &AC_AICon_Healer::FindLowestHealthAlly, ReTargetInterval, true);
}

void AC_AICon_Healer::FindLowestHealthAlly()
{
	if (!HasAuthority() || !GetWorld()){return;}
	
	AHealerCharacter* Healer = Cast<AHealerCharacter>(GetPawn());
	
	UBlackboardComponent* BB = GetBlackboardComponent();
	
	if (!Healer || !BB || Healer->HealthComponent == nullptr){return;}
	
	UHealthComponent* OwnHealth = Healer->HealthComponent;
	
	if (OwnHealth->IsDead())
	{
		BB->ClearValue(TargetActorKey);
		return;
	}
	
	TArray<AActor*> AllCharacters;
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACapstoneProjectCharacter::StaticClass(), AllCharacters);
	
	ACapstoneProjectCharacter* BestTarget = nullptr;
	float LowestHealthRatio = TNumericLimits<float>::Max();
	
	for (AActor* Actor : AllCharacters)
	{
		ACapstoneProjectCharacter* Candidate = Cast<ACapstoneProjectCharacter>(Actor);
		
		if (!IsValid(Candidate) || Candidate == Healer){continue;}
		
		if (Candidate->GetGenericTeamId().GetId() == 1){continue;}
		
		UHealthComponent* HC = Candidate->FindComponentByClass<UHealthComponent>();
		
		if (!HC || HC->IsDead()){continue;}
		
		const float MaxHealth = HC->GetMaxHealth();
		const float CurrentHealth = HC->GetCurrentHealth();
		
		UE_LOG(LogTemp, Warning, TEXT("CurrentHealth  is : %f and Current MaxHealth is : %f"), CurrentHealth, MaxHealth);
		if (MaxHealth <= 0.f || CurrentHealth >= MaxHealth){continue;}
		UE_LOG(LogTemp, Warning, TEXT("Pass4"));
		const float HealthRatio = CurrentHealth / MaxHealth;
		
		if (HealthRatio < LowestHealthRatio)
		{
			LowestHealthRatio = HealthRatio;
			BestTarget = Candidate;
		}
	}
	
	if (BestTarget)
	{
		BB->SetValueAsObject(TargetActorKey, BestTarget);
		UE_LOG(LogTemp, Warning, TEXT("BestTarget Is Set"));
	}else
	{
		BB->ClearValue(TargetActorKey);
	}
}