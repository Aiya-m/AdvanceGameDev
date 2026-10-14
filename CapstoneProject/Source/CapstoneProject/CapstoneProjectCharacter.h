// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CapstoneProjectCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UHealthComponent;
class UInputAction;

/**
 *  A controllable top-down perspective character
 */
UCLASS(abstract)
class ACapstoneProjectCharacter : public ACharacter
{
	GENERATED_BODY()

private:

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

public:

	/** Constructor */
	ACapstoneProjectCharacter();

	/** Initialization */
	virtual void BeginPlay() override;

	/** Update */
	virtual void Tick(float DeltaSeconds) override;

	/** Returns the camera component **/
	UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent.Get(); }

	/** Returns the Camera Boom component **/
	USpringArmComponent* GetCameraBoom() const { return CameraBoom.Get(); }
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;
	
	UPROPERTY(EditAnywhere, Category="Melee")
	float MeleeRange = 150.f;
	
	UPROPERTY(EditAnywhere, Category="Melee")
	float MeleeHalfAngle = 60.f;
	
	UPROPERTY(EditAnywhere, Category="Melee")
	float MeleeDamage = 25.f;
	
	UPROPERTY(VisibleAnywhere, Category="Melee")
	class USphereComponent* MeleeReach;
	
	// UPROPERTY(EditAnywhere, Category="WeaponGun")
	// float GunDamage;
	//
	// UPROPERTY(EditAnywhere, Category="WeaponGun")
	// float GunRange = 10000.f;
	//
	// UPROPERTY(EditAnywhere, Category="WeaponGun")
	// float FireRate = 0.15f;
	//
	// UPROPERTY(EditAnywhere, Category="WeaponGun")
	// bool bIsAutomatic = true;
	//
	// UPROPERTY(EditAnywhere, Category="WeaponGun")
	// class USkeletalMeshComponent* WeaponMesh;
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerMeleeAttack();
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* AttackAction;
	
	void OnAttack(const FInputActionValue& Value);
	
protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};

