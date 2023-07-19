// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"

#include "SAttributeComponent.h"
#include "SPlayerState.h"

// Sets default values
ASHealthPotion::ASHealthPotion()
{
	HealAmount = 20.0f;
	CreditsCost = 20;
}

void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}
	if (USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(InstigatorPawn))
	{
		if (ASPlayerState* PlayerState = InstigatorPawn->GetPlayerState<ASPlayerState>())
		{
			if (!AttributeComponent->IsFullHealth() && PlayerState->ApplyCreditsChange(-CreditsCost))
			{
				AttributeComponent->ApplyHealthChange(this, HealAmount);
				DeactivateAndCooldown();
			}
		}
	}
}
