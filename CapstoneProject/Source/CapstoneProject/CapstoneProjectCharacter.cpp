// Copyright Epic Games, Inc. All Rights Reserved.

#include "CapstoneProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "HealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "InputAction.h"
#include "Components/SphereComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

ACapstoneProjectCharacter::ACapstoneProjectCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create the camera boom component
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));

	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false;

	// Create the camera component
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));

	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	
	MeleeReach = CreateDefaultSubobject<USphereComponent>(TEXT("MeleeReach"));
	MeleeReach->SetupAttachment(GetMesh());
	MeleeReach->SetSphereRadius(MeleeRange);
	MeleeReach->SetRelativeLocation(FVector(0, 0, 90));
	MeleeReach->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeleeReach->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeleeReach->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	MeleeReach->SetGenerateOverlapEvents(true);
}

void ACapstoneProjectCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AttackAction)
		{
			EnhancedInputComp->BindAction(AttackAction, ETriggerEvent::Started, this, &ACapstoneProjectCharacter::OnAttack);
		}
	}
}

void ACapstoneProjectCharacter::BeginPlay()
{
	Super::BeginPlay();

	// stub
}

void ACapstoneProjectCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	// stub
}

void ACapstoneProjectCharacter::OnAttack(const FInputActionValue&)
{
	ServerMeleeAttack();
}



bool ACapstoneProjectCharacter::ServerMeleeAttack_Validate()
{
	return true;
}

void ACapstoneProjectCharacter::ServerMeleeAttack_Implementation()
{
	TArray<AActor*> Overlapping;
	MeleeReach->GetOverlappingActors(Overlapping, ACapstoneProjectCharacter::StaticClass());
	
	TSet<AActor*> AlreadyHit;
	const FVector Origin =GetActorLocation();
	const FVector Forward=GetActorForwardVector();
	const float CosThresh= FMath::Cos(FMath::DegreesToRadians(MeleeHalfAngle));
	
	bool bAnyHit= false;
	FVector HitLocation=Origin +Forward * MeleeRange;
	
	for (AActor* T:Overlapping)
	{
		if (!T|| T== this)continue;
		
		
		if (AlreadyHit.Contains(T)) continue;
		
		const FVector To= T->GetActorLocation()-Origin;
		const float Dist= To.Size();
		
		
		if (Dist>MeleeRange) continue;
		
		const FVector Dir=To.GetSafeNormal();
	
		if (FVector::DotProduct(Forward,Dir)<CosThresh) continue;
		
		AlreadyHit.Add(T);
		
		if (UHealthComponent* HC=T->FindComponentByClass<UHealthComponent>())
		{
			HC->ApplyDamage(MeleeDamage);
		}
		
		if (!bAnyHit)
		{
			bAnyHit= true;
			HitLocation= T->GetActorLocation();
		}
	}
}

float ACapstoneProjectCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	const float Applied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	if (!HealthComponent || HealthComponent->IsDead())
	{
		return Applied;
	}
	
	HealthComponent->ApplyDamage(DamageAmount);
	
	return Applied;
}
