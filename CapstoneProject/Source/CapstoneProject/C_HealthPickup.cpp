// Fill out your copyright notice in the Description page of Project Settings.


#include "C_HealthPickup.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

#include "GameFramework/Pawn.h"
#include "HealthComponent.h"

// Sets default values
AC_HealthPickup::AC_HealthPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	bReplicates = true;
	
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	
	SetRootComponent(SceneRoot);
	
	HealMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HealMesh"));
	HealMesh->SetupAttachment(SceneRoot);
	HealMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	PickUpCollision = CreateDefaultSubobject<USphereComponent>(TEXT("PickupCollision"));
	PickUpCollision->SetupAttachment(SceneRoot);
	PickUpCollision->SetSphereRadius(100.0f);
	PickUpCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PickUpCollision->SetCollisionObjectType(ECC_WorldDynamic);
	PickUpCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	PickUpCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	PickUpCollision->SetGenerateOverlapEvents(true);
	PickUpCollision->OnComponentBeginOverlap.AddDynamic( this, &AC_HealthPickup::OnHealPickUpOverlap);
}

// Called when the game starts or when spawned
void AC_HealthPickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AC_HealthPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HealMesh)
	{
		HealMesh->AddLocalRotation(
			FRotator(
				0.0f,
				RotationSpeed * DeltaTime,
				0.0f
			)
		);
	}
}

void AC_HealthPickup::OnHealPickUpOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority()){return;}
	if (!OtherActor){return;}
	
	APawn* PlayerPawn = Cast<APawn>(OtherActor);
	
	if (!PlayerPawn || !PlayerPawn->IsPlayerControlled()){return;}
	
	UHealthComponent* HealthComponent = PlayerPawn->FindComponentByClass<UHealthComponent>();
	
	if (!HealthComponent){return;}
	if (HealthComponent->IsDead()){return;}
	
	const float OldHealth = HealthComponent->GetCurrentHealth();
	const float MaxHealth = HealthComponent->GetMaxHealth();
	
	if (OldHealth >= MaxHealth){return;}
	
	HealthComponent->Heal(HealthAmount);
	
	const float NewHealth = HealthComponent->GetCurrentHealth();
	
	if(NewHealth > OldHealth)
	{
		Destroy();
	}
}

