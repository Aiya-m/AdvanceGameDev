// Fill out your copyright notice in the Description page of Project Settings.


#include "C_AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Damage.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

AC_AIController::AC_AIController()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight"));
	
	SightConfig->SightRadius = 1200.f;
	SightConfig->LoseSightRadius = 1500.f;
	SightConfig->PeripheralVisionAngleDegrees = 60.f;
	
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = false;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
	
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception")));
	
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
	
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing"));
	HearingConfig->HearingRange = 3000.f;
	HearingConfig->SetMaxAge(8.f);
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = false;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = false;
	
	GetPerceptionComponent()->ConfigureSense(*HearingConfig);
	
	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("Damage"));
	DamageConfig->SetMaxAge(8.f);
	GetPerceptionComponent()->ConfigureSense(*DamageConfig);
	
	GetPerceptionComponent()->SetDominantSense(SightConfig->GetSenseImplementation());
}

void AC_AIController::BeginPlay()
{
	Super::BeginPlay();
	
	const APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn)
	{
		const ETeamAttitude::Type Att = GetTeamAttitudeTowards(*PlayerPawn);
		
		UE_LOG(LogTemp, Warning, TEXT("AI TEAM: %d | ATTITUDE TO PLAYER: %s"),
		(int32)GetGenericTeamId().GetId(), Att == ETeamAttitude::Hostile ? TEXT("Hostile") : Att == ETeamAttitude::Friendly ? TEXT("Friendly") : TEXT("Neutral"));
	}
	
	if (UAIPerceptionComponent* Perception = GetPerceptionComponent())
	{
		Perception->OnTargetPerceptionUpdated.AddDynamic(this, &AC_AIController::OnTargetSensed);
	}
}

void AC_AIController::OnTargetSensed(AActor* Actor, FAIStimulus Stimulus)
{
    UBlackboardComponent* BB = GetBlackboardComponent();

    if (BB == nullptr || Actor == nullptr)
    {
        return;
    }

    if (!Actor->ActorHasTag(FName(TEXT("Player"))))
    {
        return;
    }
	
	const TSubclassOf<UAISense> Sense = UAIPerceptionSystem::GetSenseClassForStimulus(this, Stimulus);
	
	if (Sense == UAISense_Sight::StaticClass())
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			BB->SetValueAsObject(TargetActorKey, Actor);
		}
		else
		{
			BB->ClearValue(TargetActorKey);
		}
		return;
	}
	
	if (Sense == UAISense_Damage::StaticClass())
	{
		BB->SetValueAsObject(TargetActorKey, Actor);
		return;
	}
	
	if (Sense == UAISense_Hearing::StaticClass())
	{
		BB->SetValueAsVector(InvestigateLocationKey, Stimulus.StimulusLocation);
	}
}

void AC_AIController::OnPossess(class APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
	}
}

FGenericTeamId AC_AIController::GetGenericTeamId() const
{
	return FGenericTeamId(1);
}