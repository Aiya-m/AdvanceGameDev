// Fill out your copyright notice in the Description page of Project Settings.


#include "C_AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"

AC_AIController::AC_AIController()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight"));
	
	SightConfig->SightRadius = 1200.f;
	SightConfig->LoseSightRadius = 1500.f;
	SightConfig->PeripheralVisionAngleDegrees = 60.f;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception")));
	
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
}

void AC_AIController::BeginPlay()
{
	Super::BeginPlay();
	
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

    if (Stimulus.WasSuccessfullySensed())
    {
        BB->SetValueAsObject(TargetActorKey, Actor);
    }
    else
    {
        BB->ClearValue(TargetActorKey);
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