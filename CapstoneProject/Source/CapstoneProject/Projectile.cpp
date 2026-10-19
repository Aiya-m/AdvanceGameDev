// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "CapstoneProjectCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HealthComponent.h"
#include "Weapon.h"


AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	InitialLifeSpan = 2.f;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	
	//Bind to Event
	CollisionComp->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	RootComponent = CollisionComp;

	// สร้าง Projectile Movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.0f; // bullet go straight
}

void AProjectile::SetDamage(float newdamage)
{
	Damage = newdamage;
}

void AProjectile::BulletMakeShot_Implementation(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this && OtherActor != GetInstigator())
	{
		AController* InstigatorController = GetInstigator() ? GetInstigator()->GetController() : nullptr;
		if (HasAuthority())
		{
			UGameplayStatics::ApplyDamage(
				OtherActor,
				Damage,
				InstigatorController,
				this,
				UDamageType::StaticClass()
			);
			
			Destroy();
			return;
		}
	}
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	BulletMakeShot(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
}
