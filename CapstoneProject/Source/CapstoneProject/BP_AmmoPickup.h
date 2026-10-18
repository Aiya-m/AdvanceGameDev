// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BP_AmmoPickup.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class USphereComponent;
class UPrimitiveComponent;

struct FHitResult;

UCLASS()
class CAPSTONEPROJECT_API ABP_AmmoPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABP_AmmoPickup();
	
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PickUp")
	USceneComponent* SceneRoot;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PickUp")
	UStaticMeshComponent* PickUpMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PickUp")
	USphereComponent* PickUpCollision;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="PickUp", meta = (ClampMin = "1"))
	int32 AmmoAmount = 30;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="PickUp")
	float RotationSpeed = 90.0f;
	
	UFUNCTION()
	void OnPickUpOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	

};
