// Fill out your copyright notice in the Description page of Project Settings.


#include "SDashProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ASDashProjectile::ASDashProjectile()
{
	ProjectileMovementComponent->InitialSpeed = 6000.0f;

	TeleportDelay = 0.2f;
	DetonateDelay = 0.2f;
}

void ASDashProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ASDashProjectile::BeginPlay()
{
	Super::BeginPlay();
	// SphereComponent->IgnoreActorWhenMoving(GetInstigator(), true);
	GetWorldTimerManager().SetTimer(TimerHandleDash, this, &ASDashProjectile::Explode, DetonateDelay);
}

void ASDashProjectile::Explode_Implementation()
{
	GetWorldTimerManager().ClearTimer(TimerHandleDash);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleTemplate, GetActorLocation(), GetActorRotation());
	ParticleSystemComponent->DeactivateSystem();
	ProjectileMovementComponent->StopMovementImmediately();
	SetActorEnableCollision(false);
	GetWorldTimerManager().SetTimer(TimerHandleDash, this, &ASDashProjectile::Teleport, TeleportDelay);
}

void ASDashProjectile::Teleport()
{
	if (AActor* PlayerInstigator = GetInstigator())
	{
		PlayerInstigator->TeleportTo(GetActorLocation(), PlayerInstigator->GetActorRotation(), false, true);
	}
	Destroy();
}


