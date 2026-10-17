// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Projectile.h"
#include "NavigationSystemTypes.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	bReplicates = true;
	
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);
	
	WeaponMeshStatic = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMeshStatic"));
	WeaponMeshStatic->SetupAttachment(RootComp);
	WeaponMeshStatic->SetCollisionProfileName(TEXT("NoCollision"));
	SetBullet();
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeapon::fire()
{
	FVector SpawnLocation = GetMuzzleLocation();
	FRotator SpawnRotation = GetMuzzleForwardVector().Rotation();
	Server_fire(SpawnLocation, SpawnRotation);
}

void AWeapon::SetBullet()
{
	bullets = WeaponMagazine;
}

void AWeapon::FireBullet()
{
	if (bullets > 0)
	{
		bullets--;
		return;
	}
	bullets = 0;
	return;
}

void AWeapon::AddBullet(int32 bullet)
{
	if (bullets >= WeaponMagazine)
	{
		bullets = WeaponMagazine;
		return;
	}
	bullets += bullet;
	return;
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::Server_fire_Implementation(FVector SpawnLocation, FRotator SpawnRotation)
{
	if (HasAuthority() && ProjectileClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ProjectileClass"));
		// FVector End = Start + (ForwardVector * WeaponRange);
	
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = GetInstigator();
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
		UWorld* World = GetWorld();
		if (World)
		{
			if (bullets > 0)
			{
				AProjectile* SpawnedBullet = World->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
                			
                if (SpawnedBullet)
                {
                	SpawnedBullet->SetDamage(BaseDamage);
                	FireBullet();
                	UE_LOG(LogTemp, Warning, TEXT("Spawnbullet"));
                }
			}
			 else
			{
				UE_LOG(LogTemp, Warning, TEXT("Out of Bullet"));
			}
		}	
	}
	
	// Multicast_PlayFireFX();
}

void AWeapon::StartFire()
{
	fire(); 

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(
			FireTimerHandle,
			this,
			&AWeapon::fire,
			FireRate,
			true
		);
	}
}

void AWeapon::StopFire()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(FireTimerHandle);
	}
}


FVector AWeapon::GetMuzzleLocation() const
{
	if (WeaponMeshStatic && WeaponMeshStatic->GetStaticMesh())
	{
		return WeaponMeshStatic->GetSocketLocation(MuzzleSocketName);
	}
	
	return GetActorLocation();
}

FVector AWeapon::GetMuzzleForwardVector() const
{
	if (WeaponMeshStatic && WeaponMeshStatic->GetStaticMesh())
	{
		return WeaponMeshStatic->GetSocketRotation(MuzzleSocketName).Vector();
	}
	
	return GetActorForwardVector();
}
