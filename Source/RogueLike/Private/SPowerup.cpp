// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerup.h"
#include "SAttributeComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASPowerup::ASPowerup()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	SetRootComponent(StaticMeshComponent);
	
	RespawnTime = 10.0f;

	bReplicates = true;
	bIsActive = true;
}


void ASPowerup::Interact_Implementation(APawn* InstigatorPawn)
{
}

void ASPowerup::Activate()
{
	bIsActive = true;
	OnRep_SetState();
}

void ASPowerup::DeactivateAndCooldown()
{
	bIsActive = false;
	OnRep_SetState();
	FTimerHandle CooldownTimer;
	GetWorldTimerManager().SetTimer(CooldownTimer, this, &ASPowerup::Activate, RespawnTime);
}

void ASPowerup::OnRep_SetState()
{
	SetActorEnableCollision(bIsActive);
	StaticMeshComponent->SetVisibility(bIsActive, true);
}

void ASPowerup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASPowerup, bIsActive);
}

