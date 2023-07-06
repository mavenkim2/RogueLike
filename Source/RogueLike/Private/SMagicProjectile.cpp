// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"

#include "Projects.h"
#include "SAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// SphereComponent = CreateDefaultSubobject<USphereComponent>("Sphere Component");
	// RootComponent = SphereComponent;
	// SphereComponent->SetCollisionProfileName("Projectile");

	// ParticleSystemComponent	 = CreateDefaultSubobject<UParticleSystemComponent>("Particle Component");
	// ParticleSystemComponent->SetupAttachment(RootComponent);
	//
	// ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("Movement Component");
	ProjectileMovementComponent->InitialSpeed = 1000.0f;
	// ProjectileMovementComponent->bRotationFollowsVelocity = true;
	// ProjectileMovementComponent->bInitialVelocityInLocalSpace = true;
	
}

// Called when the game starts or when spawned
void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::Explode);
}

void ASMagicProjectile::Explode(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		if (USAttributeComponent* AttributeComponent = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass())))
		{
			AttributeComponent->ApplyHealthChange(-20.0f);
			Destroy();
		}
	}
}


