// Fill out your copyright notice in the Description page of Project Settings.


#include "SDashProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ASDashProjectile::ASDashProjectile()
{
	// SphereComponent = CreateDefaultSubobject<USphereComponent>("Sphere Component");
	// RootComponent = SphereComponent;
	// SphereComponent->SetCollisionProfileName("Projectile");
	//
	// ParticleSystemComponent	 = CreateDefaultSubobject<UParticleSystemComponent>("Particle Component");
	// ParticleSystemComponent->SetupAttachment(RootComponent);
	//
	// ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("Movement Component");
	ProjectileMovementComponent->InitialSpeed = 6000.0f;
	// ProjectileMovementComponent->bRotationFollowsVelocity = true;
	// ProjectileMovementComponent->bInitialVelocityInLocalSpace = true;

	ParticleTemplate = CreateDefaultSubobject<UParticleSystem>("Particle Template");
}

void ASDashProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComponent->OnComponentHit.AddDynamic(this, &ASDashProjectile::DetonateCollision);
	
}

void ASDashProjectile::BeginPlay()
{
	Super::BeginPlay();
	SphereComponent->IgnoreActorWhenMoving(GetInstigator(), true);
	GetWorldTimerManager().SetTimer(TimerHandleDash, this, &ASDashProjectile::Detonate, 0.2f);
}

void ASDashProjectile::DetonateCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Detonate();
}

void ASDashProjectile::Detonate()
{
	GetWorldTimerManager().ClearTimer(TimerHandleDash);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleTemplate, GetActorLocation(), GetActorRotation());
	ProjectileMovementComponent->StopMovementImmediately();
	SetActorEnableCollision(false);
	GetWorldTimerManager().SetTimer(TimerHandleDash, this, &ASDashProjectile::Teleport, 0.2f);
}

void ASDashProjectile::Teleport()
{
	if (AActor* PlayerInstigator = GetInstigator())
	{
		if (!PlayerInstigator->TeleportTo(GetActorLocation(), PlayerInstigator->GetActorRotation(), false, false))
		{
			UE_LOG(LogTemp, Warning, TEXT("Teleport didn't work lol!!!"));
		}
	}
	Destroy();
}


