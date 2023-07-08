// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"

#include "SAttributeComponent.h"

// Sets default values
ASHealthPotion::ASHealthPotion()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	SetRootComponent(StaticMeshComponent);

	HealAmount = 20.0f;
	Cooldown = 10.0f;
	bIsActive = true;
	
}

void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!bIsActive)
	{
		return;
	}
	if (USAttributeComponent* AttributeComponent = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass())))
	{
		if (AttributeComponent->ApplyHealthChange(HealAmount))
		{
			GetWorldTimerManager().SetTimer(CooldownTimer, this, &ASHealthPotion::Activate, Cooldown);
			Deactivate();
		}
	}
}

void ASHealthPotion::Activate()
{
	bIsActive = true;
	StaticMeshComponent->SetVisibility(true);
}

void ASHealthPotion::Deactivate()
{
	bIsActive = false;
	StaticMeshComponent->SetVisibility(false);
}

