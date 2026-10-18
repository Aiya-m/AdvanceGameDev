// Fill out your copyright notice in the Description page of Project Settings.


#include "HealerCharacter.h"

#include "HealthComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

AHealerCharacter::AHealerCharacter()
{
	bReplicates = true;
	SetReplicateMovement(true);
	
	HealingZone = CreateDefaultSubobject<USphereComponent>(TEXT("HealingZone"));
	
	HealingZone->SetupAttachment(RootComponent);
	HealingZone->InitSphereRadius(HealingRadius);
	HealingZone->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AHealerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (!HasAuthority()) {return;}
	
	HealingZone->SetSphereRadius(HealingRadius);
	
	GetWorld()->GetTimerManager().SetTimer(HealingTimerHandle, this, &AHealerCharacter::HealAlliesInZone, HealInterval, true);
}

void AHealerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(HealingTimerHandle);
	}
	Super::EndPlay(EndPlayReason);
}

void AHealerCharacter::HealAlliesInZone()
{
	if (!HasAuthority() || !GetWorld()){return;}
	TArray<AActor*> AllCharacters;
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACapstoneProjectCharacter::StaticClass(), AllCharacters);
	
	const FVector ZoneLocation = GetActorLocation();
	
	for (AActor* Actor : AllCharacters)
	{
		ACapstoneProjectCharacter* Ally = Cast<ACapstoneProjectCharacter>(Actor);
		
		if (!IsValid(Ally) || Ally == this){continue;}
		if (Ally->GetGenericTeamId().GetId() == 1){continue;}
		
		const float Distance = FVector::Dist(ZoneLocation, Ally->GetActorLocation());
		
		if (Distance > HealingRadius){continue;}
		
		UHealthComponent* HC = Ally->FindComponentByClass<UHealthComponent>();
		
		if (!HC || HC->IsDead()){continue;}
		
		HC->Heal(HealAmount);
	}
}
