// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	Health = 100;
	HealthMax = 100;
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* Actor)
{
	USAttributeComponent* AttributeComponent = Actor->GetComponentByClass<USAttributeComponent>();
	if (AttributeComponent)
	{
		return AttributeComponent;
	}
	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	USAttributeComponent* AttributeComponent = GetAttributes(Actor);
	if (AttributeComponent)
	{
		return AttributeComponent->IsAlive();
	}
	return false;
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	float OldHealth = Health;
	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);
	float ActualDelta = Health-OldHealth;
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);
	return ActualDelta != 0;
}


