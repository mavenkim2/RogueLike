// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectileBaseClass.h"

#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"

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

	FlightSoundComponent = CreateDefaultSubobject<UAudioComponent>("Flight Sound");
	FlightSoundComponent->SetupAttachment(RootComponent);

	SphereComponent->IgnoreActorWhenMoving(GetInstigator(), true);

	CameraShakeInnerRadius = 250.f;
	CameraShakeOuterRadius = 2500.f;
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
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), GetActorRotation());
		if (ensure(CameraShake))
		{
			UGameplayStatics::PlayWorldCameraShake(this, CameraShake, GetActorLocation(), CameraShakeInnerRadius, CameraShakeOuterRadius);
		}
		Destroy();
	}
}
