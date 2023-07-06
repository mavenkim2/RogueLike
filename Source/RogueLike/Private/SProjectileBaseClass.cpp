// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectileBaseClass.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
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
}

// Called when the game starts or when spawned
void ASProjectileBaseClass::BeginPlay()
{
	Super::BeginPlay();
}
