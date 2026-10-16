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
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::Fire()
{
	Server_Fire();
}

void AWeapon::Server_Fire_Implementation()
{
	if (ProjectileClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ProjectileClass"));
		FVector SpawnLocation = GetMuzzleLocation();
		FRotator SpawnRotator = GetMuzzleForwardVector().Rotation();
		// FVector End = Start + (ForwardVector * WeaponRange);
	
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = GetInstigator();
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
		UWorld* World = GetWorld();
		if (World)
		{
			AProjectile* SpawnedBullet = World->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotator, SpawnParams);
			
			if (SpawnedBullet)
			{
				SpawnedBullet->SetDamage(BaseDamage);
				UE_LOG(LogTemp, Warning, TEXT("Spawnbullet"));
			}
		}	
	}
	
	// Multicast_PlayFireFX();
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
