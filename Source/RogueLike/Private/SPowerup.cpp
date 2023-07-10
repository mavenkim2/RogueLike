// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerup.h"
#include "SAttributeComponent.h"

// Sets default values
ASPowerup::ASPowerup()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	SetRootComponent(StaticMeshComponent);
	
	RespawnTime = 10.0f;
}

void ASPowerup::Interact_Implementation(APawn* InstigatorPawn)
{
}

void ASPowerup::SetState(bool bIsActivated)
{
	SetActorEnableCollision(bIsActivated);
	StaticMeshComponent->SetVisibility(bIsActivated, true);
}

void ASPowerup::Activate()
{
	SetState(true);
}

void ASPowerup::DeactivateAndCooldown()
{
	SetState(false);
	FTimerHandle CooldownTimer;
	GetWorldTimerManager().SetTimer(CooldownTimer, this, &ASPowerup::Activate, RespawnTime);
}
