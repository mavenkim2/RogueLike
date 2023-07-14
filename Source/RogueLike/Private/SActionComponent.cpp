// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionComponent.h"

#include "SAction.h"


// Sets default values for this component's properties
USActionComponent::USActionComponent()
{
}

void USActionComponent::BeginPlay()
{
	Super::BeginPlay();
	for (TSubclassOf<USAction> ActionClass : DefaultActions)
	{
		AddAction(ActionClass);
	}
}

void USActionComponent::AddAction(TSubclassOf<USAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}
	USAction* NewAction = NewObject<USAction>(this, ActionClass);
	if (ensure(NewAction))
	{
		Actions.Add(NewAction->GetActionName(), NewAction);
	}
}

bool USActionComponent::StartActionByName(AActor* InstigatorActor, FName ActionName)
{
	if (USAction* Action = Actions[ActionName])
	{
		Action->StartAction(InstigatorActor);
		return true;
	}
	return false;
}

bool USActionComponent::StopActionByName(AActor* InstigatorActor, FName ActionName)
{
	if (USAction* Action = Actions[ActionName])
	{
		Action->StopAction(InstigatorActor);
		return true;
	}
	return false;
}
