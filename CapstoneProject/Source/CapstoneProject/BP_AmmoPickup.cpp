// Fill out your copyright notice in the Description page of Project Settings.


#include "BP_AmmoPickup.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

#include "Weapon.h"
#include "CapstoneProjectCharacter.h"

// Sets default values
ABP_AmmoPickup::ABP_AmmoPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	bReplicates = true;
	
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	
	SetRootComponent(SceneRoot);
	
	PickUpMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickUpMesh"));
	PickUpMesh->SetupAttachment(SceneRoot);
	PickUpMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	PickUpCollision = CreateDefaultSubobject<USphereComponent>(TEXT("PickUpCollision"));
	PickUpCollision->SetupAttachment(SceneRoot);
	PickUpCollision->SetSphereRadius(100.0f);
	PickUpCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PickUpCollision->SetCollisionObjectType(ECC_WorldDynamic);
	PickUpCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	PickUpCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	PickUpCollision->SetGenerateOverlapEvents(true);
	PickUpCollision->OnComponentBeginOverlap.AddDynamic(this, &ABP_AmmoPickup::OnPickUpOverlap);

}

// Called when the game starts or when spawned
void ABP_AmmoPickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABP_AmmoPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (PickUpMesh)
	{
		PickUpMesh->AddLocalRotation(
			FRotator(
				0.0f,
				RotationSpeed * DeltaTime,
				0.0f
			)
		);
	}

}

void ABP_AmmoPickup::OnPickUpOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority()){return;}
	
	if (!OtherActor){return;}
	
	ACapstoneProjectCharacter* Player = Cast<ACapstoneProjectCharacter>(OtherActor);
	
	if (!Player){return;}
	
	if (Player->GetGenericTeamId().GetId() != 0){return;}
	
	TArray<AActor*> AttachedActors;
	Player->GetAttachedActors(AttachedActors);
	
	for (AActor* AttachedActor : AttachedActors)
	{
		AWeapon* Weapon = Cast<AWeapon>(AttachedActor);
		
		if (!Weapon){continue;}
		
		bool bAmmoAdded = Weapon->AddBullet(AmmoAmount);
		
		if (bAmmoAdded){Destroy();};
		
		return;
	}
}
