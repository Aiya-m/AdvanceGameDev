// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.h"
#include "Weapon.generated.h"

UCLASS()
class CAPSTONEPROJECT_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();
	
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void FireBullet();
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StartFire();
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StopFire();
	
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void AddBullet(int32 bullet);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* RootComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* WeaponMeshStatic;

	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	// USkeletalMeshComponent* WeaponMeshSkeletal;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Stats")
	TSubclassOf<AProjectile> ProjectileClass;
	
	UFUNCTION()
	void fire();
	
	UFUNCTION(Server, Reliable)
	void Server_fire(FVector SpawnLocation, FRotator SpawnRotation);
	
	FTimerHandle FireTimerHandle;
	
	// UFUNCTION(NetMulticast, Reliable)
	// void Multicast_PlayFireFX(FVector HitLocation, bool bDidHit);
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	FVector GetMuzzleLocation() const;

	// ฟังก์ชั่นดึงทิศทางปลายกระบอกปืน
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	FVector GetMuzzleForwardVector() const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Stats")
	FString WeaponName = "weapon";
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Stats")
	float BaseDamage = 20.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Stats")
	float FireRate = 0.02f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Stats")
	float WeaponRange = 5000.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Stats")
	int32 WeaponMagazine = 20;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Stats")
	int32 bullets = WeaponMagazine;
	
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void SetBullet();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Stats")
	FName MuzzleSocketName = FName("Muzzle");

	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Visuals")
	// USoundBase* FireSound;
	//
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Visuals")
	// UNiagaraSystem* MuzzleFX;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
