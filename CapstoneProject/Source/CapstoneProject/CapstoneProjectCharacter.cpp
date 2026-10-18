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
#include "EnemyProjectile.h"
#include "GameFramework/Actor.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Weapon.h"

AWeapon* Weapon;

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
	
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

void ACapstoneProjectCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AttackAction)
		{
			EnhancedInputComp->BindAction(AttackAction, ETriggerEvent::Started, this, &ACapstoneProjectCharacter::OnAttack);

			EnhancedInputComp->BindAction(AttackAction, ETriggerEvent::Completed, this, &ACapstoneProjectCharacter::StopAttack);
			EnhancedInputComp->BindAction(AttackAction, ETriggerEvent::Canceled, this, &ACapstoneProjectCharacter::StopAttack);
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



void ACapstoneProjectCharacter::OnAttack()
{
	if (Weapon)
	{
		UE_LOG(LogTemp, Error, TEXT("OnAttack"));
		Weapon->StartFire();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("OnAttack Failed: Weapon is NULL! Please equip a weapon first."));
	}
}

void ACapstoneProjectCharacter::StopAttack()
{
	if (Weapon)
	{
		Weapon->StopFire();
	}
}

void ACapstoneProjectCharacter::EquipWeapon(AWeapon* NewWeapon)
{
	if (!NewWeapon) return;

	Weapon = NewWeapon;
	Weapon->SetOwner(this);
	Weapon->SetInstigator(this);

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	Weapon->AttachToComponent(GetMesh(), AttachmentRules, TEXT("WeaponSocket"));
}

void ACapstoneProjectCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACapstoneProjectCharacter, Weapon);
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

FGenericTeamId ACapstoneProjectCharacter::GetGenericTeamId() const
{
	return FGenericTeamId(0);
}


void ACapstoneProjectCharacter::FireProjectileAt(AActor* Target)
{
	
	if (!HasAuthority() || !IsValid(Target)){return;}

	if (!EnemyProjectileClass)
	{
		UE_LOG(LogTemp,Warning,TEXT("ProjectileClass is not assigned!"));
		return;
	}

	const float Now = GetWorld()->GetTimeSeconds();

	if (Now - LastAttackTime < MinAttackInterval)
	{
		return;
	}

	LastAttackTime = Now;

	const FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.f + FVector(0.f, 0.f, 50.f);

	const FVector Direction = (Target->GetActorLocation() - SpawnLocation).GetSafeNormal();

	const FRotator SpawnRotation = Direction.Rotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AEnemyProjectile* Projectile = GetWorld()->SpawnActor<AEnemyProjectile>(EnemyProjectileClass,SpawnLocation,SpawnRotation,SpawnParams);

	if (Projectile)
	{
		Projectile->InitializeComponent(this, 10.f);

		Projectile->SetActorRotation(SpawnRotation);

		if (UProjectileMovementComponent* Movement = Projectile->FindComponentByClass<UProjectileMovementComponent>())
		{
			Movement->Velocity = Direction * Movement->InitialSpeed;
		}
	}
}
