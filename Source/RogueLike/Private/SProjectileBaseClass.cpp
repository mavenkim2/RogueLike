// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectileBaseClass.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASProjectileBaseClass::ASProjectileBaseClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	SphereComponent = CreateDefaultSubobject<USphereComponent>("Sphere Component");
	RootComponent = SphereComponent;
	SphereComponent->SetCollisionProfileName("Projectile");
	
	ParticleSystemComponent	 = CreateDefaultSubobject<UParticleSystemComponent>("Particle Component");
	ParticleSystemComponent->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("Movement Component");
	ProjectileMovementComponent->InitialSpeed = 6000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bInitialVelocityInLocalSpace = true;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	ParticleTemplate = CreateDefaultSubobject<UParticleSystem>("Particle Template");
}

// Called when the game starts or when spawned
void ASProjectileBaseClass::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComponent->OnComponentHit.AddDynamic(this, &ASProjectileBaseClass::OnActorHit);
}

void ASProjectileBaseClass::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}

void ASProjectileBaseClass::Explode_Implementation()
{
	if (ensure(IsValid(this)))
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ParticleTemplate, GetActorLocation(), GetActorRotation());
		Destroy();
	}
}
