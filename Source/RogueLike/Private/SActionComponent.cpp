// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionComponent.h"

#include "SAction.h"


// Sets default values for this component's properties
USActionComponent::USActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

FGameplayTagContainer& USActionComponent::GetActiveGameplayTags()
{
	return ActiveGameplayTags;
}

void USActionComponent::BeginPlay()
{
	Super::BeginPlay();
	for (TSubclassOf<USAction> ActionClass : DefaultActions)
	{
		AddAction(GetOwner(), ActionClass);
	}
}

void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FString DebugString = FString::Printf(TEXT("%s: %s"), *GetNameSafe(GetOwner()), *ActiveGameplayTags.ToStringSimple());
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::White, DebugString);
}

void USActionComponent::AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}
	USAction* NewAction = NewObject<USAction>(this, ActionClass);
	if (ensure(NewAction))
	{
		Actions.Add(NewAction->GetActionName(), NewAction);
		if (NewAction->bAutoStart && NewAction->CanStart(Instigator))
		{
			NewAction->StartAction(Instigator);
		}
	}
}

void USActionComponent::RemoveAction(USAction* ActionToRemove)
{
	FName ActionName = ActionToRemove->GetActionName();
	if (Actions.Contains(ActionName))
	{
		Actions.Remove(ActionName);
	}
}

bool USActionComponent::StartActionByName(AActor* InstigatorActor, FName ActionName)
{
	if (USAction* Action = Actions[ActionName])
	{
		if (!Action->CanStart(InstigatorActor))
		{
			FString FailedMsg = FString::Printf(TEXT("Failed to run %s"), *ActionName.ToString());
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FailedMsg);
			return false;
		}
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
