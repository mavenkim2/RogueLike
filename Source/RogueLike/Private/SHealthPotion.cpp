// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"

#include "SAttributeComponent.h"

// Sets default values
ASHealthPotion::ASHealthPotion()
{
	HealAmount = 20.0f;
}

void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}
	if (USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(InstigatorPawn))
	{
		if (AttributeComponent->ApplyHealthChange(this, HealAmount))
		{
			DeactivateAndCooldown();
		}
	}
}
