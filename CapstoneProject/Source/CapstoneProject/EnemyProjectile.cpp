// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Pawn.h"
#include "GenericTeamAgentInterface.h"
#include "HealthComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

// Sets default values
AEnemyProjectile::AEnemyProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetReplicateMovement(true);
	
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	SetRootComponent(CollisionSphere);
	
	CollisionSphere->InitSphereRadius(12.f);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionSphere->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionSphere->SetCollisionResponseToAllChannels(ECR_Overlap);
	CollisionSphere->SetGenerateOverlapEvents(true);
	
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyProjectile::OnProjectileOverlap);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	
	ProjectileMovement->InitialSpeed = 1500.f;
	ProjectileMovement->MaxSpeed = 1500.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	
	InitialLifeSpan = LifeSpan;
}

// Called when the game starts or when spawned
void AEnemyProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyProjectile::InitializeComponent(AActor* InInstigator, float InDamage)
{
	if (!HasAuthority()){return;}
	
	Damage = InDamage;
	
	if (APawn* InstigatorPawn = Cast<APawn>(InInstigator))
	{
		SetInstigator(InstigatorPawn);
	}
	
	if (IsValid(InInstigator))
	{
		CollisionSphere->IgnoreActorWhenMoving(InInstigator, true);
	}
}

void AEnemyProjectile::OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority()){return;}
	
	if (!IsValid(OtherActor) || OtherActor == this || OtherActor == GetInstigator()){return;}
	
	const IGenericTeamAgentInterface* TeamActor = Cast<IGenericTeamAgentInterface>(OtherActor);
	
	if (TeamActor)
	{
		const FGenericTeamId TeamId = TeamActor->GetGenericTeamId();
		
		if (TeamId.GetId() == 0)
		{
			if (UHealthComponent* HC = OtherActor->FindComponentByClass<UHealthComponent>())
			{
				if (!HC->IsDead())
				{
					HC->ApplyDamage(Damage);
				}
			}
		}
	}
	Destroy();
}
