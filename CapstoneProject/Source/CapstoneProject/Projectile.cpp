// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true; // รองรับ Multiplayer

	// สร้าง Collision Component
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	CollisionComp->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	RootComponent = CollisionComp;

	// สร้าง Projectile Movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.0f; // กระสุนวิ่งตรงไม่ย้อย
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// เช็คว่าไม่ได้ยิงใส่ตัวเอง และ มี Actor ที่โดนยิง
	if (OtherActor && OtherActor != this && OtherActor != GetInstigator())
	{
		// ทำ Damage ใส่เป้าหมาย
		UGameplayStatics::ApplyDamage(OtherActor, 25.0f, GetInstigatorController(), this, UDamageType::StaticClass());

		// ลบลูกกระสุนออกเมื่อชน
		Destroy();
	}
}