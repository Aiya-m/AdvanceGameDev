// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_HealthPickup.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class USphereComponent;
class UPrimitiveComponent;

struct FHitResult;

UCLASS()
class CAPSTONEPROJECT_API AC_HealthPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AC_HealthPickup();
	
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Heal")
	USceneComponent* SceneRoot;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Heal")
	UStaticMeshComponent* HealMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Heal")
	USphereComponent* PickUpCollision;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Heal", meta = (ClampMin = "1.0"))
	float HealthAmount = 10.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Heal")
	float RotationSpeed = 90.0f;
	
	UFUNCTION()
	void OnHealPickUpOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
