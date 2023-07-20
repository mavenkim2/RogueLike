// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerup_Action.h"

#include "SAction.h"
#include "SActionComponent.h"

void ASPowerup_Action::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn && ActionToGrant))
	{
		return;
	}
	// make sure the instigator doesn't have the action already, if it does, return
	// otherwise, add action
	USActionComponent* ActionComponent = InstigatorPawn->GetComponentByClass<USActionComponent>();
	if (ActionComponent->GetAction(ActionToGrant))
	{
		return;
	}
	ActionComponent->AddAction(InstigatorPawn, ActionToGrant);
	Destroy();
}
