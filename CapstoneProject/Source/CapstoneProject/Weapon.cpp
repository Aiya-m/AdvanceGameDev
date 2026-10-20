// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Projectile.h"
#include "NavigationSystemTypes.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

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

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeapon, bullets);
	DOREPLIFETIME(AWeapon, SpareAmmo);
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

bool AWeapon::AddBullet(int32 bullet)
{
	if (WeaponName == "Pistol") {return false;}
	SpareAmmo += bullet;
	OnAmmoChanged.Broadcast(bullets, SpareAmmo);
	return true;
}

void AWeapon::Reload()
{
	Server_Reload();
}

void AWeapon::Server_Reload_Implementation()
{
	if (WeaponName == "Pistol") {return;}
	int requireAmmo = WeaponMagazine-bullets;
	if (SpareAmmo >= requireAmmo)
	{
		bullets += requireAmmo;
		SpareAmmo -= requireAmmo;
	} else if (SpareAmmo < requireAmmo)
	{
		bullets += SpareAmmo;
		SpareAmmo = 0;
	}
	OnAmmoChanged.Broadcast(bullets, SpareAmmo);
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

void AWeapon::FireBullet()
{
	if (bullets-1 == 0)
	{
		bullets = 0;
		OnAmmoChanged.Broadcast(bullets, SpareAmmo);
		return;
	}
	bullets--;
	OnAmmoChanged.Broadcast(bullets, SpareAmmo);
	return;
}

void AWeapon::OnRep_Bullets()
{
	UE_LOG(LogTemp, Warning, TEXT("bulletchange"));
	OnAmmoChanged.Broadcast(bullets, SpareAmmo);
}

void AWeapon::OnRep_SpareAmmo()
{
	UE_LOG(LogTemp, Warning, TEXT("spareammochange"));
	OnAmmoChanged.Broadcast(bullets, SpareAmmo);
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
