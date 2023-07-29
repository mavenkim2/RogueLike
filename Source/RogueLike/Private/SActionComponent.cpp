// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionComponent.h"

#include "SAction.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "RogueLike/RogueLike.h"

DECLARE_CYCLE_STAT(TEXT("StartActionByName"), STAT_StartActionByName, STATGROUP_Roguelike);

// Sets default values for this component's properties
USActionComponent::USActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}



FGameplayTagContainer& USActionComponent::GetActiveGameplayTags()
{
	return ActiveGameplayTags;
}

void USActionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority())
	{
		for (TSubclassOf<USAction> ActionClass : DefaultActions)
		{
			AddAction(GetOwner(), ActionClass);
		}
	}
}

void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// FString DebugString = FString::Printf(TEXT("%s: %s"), *GetNameSafe(GetOwner()), *ActiveGameplayTags.ToStringSimple());
	// GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::White, DebugString);
	
	// for (USAction* Action : Actions)
	// {
	// 	FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;
	// 	FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s"),
	// 		*GetNameSafe(GetOwner()),
	// 		*GetNameSafe(Action));
	// 	LogOnScreen(this, ActionMsg, TextColor, 0.f);
	// }
}

USAction* USActionComponent::GetAction(TSubclassOf<USAction> ActionClass)
{
	for (USAction* Action : Actions)
	{
		if (Action->IsA(ActionClass))
		{
			return Action;
		}
	}
	return nullptr;
}


void USActionComponent::AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	if (!GetOwner()->HasAuthority())
	{
		return;
	}
	USAction* NewAction = NewObject<USAction>(this, ActionClass);
	if (ensure(NewAction))
	{
		Actions.Add(NewAction);
		if (NewAction->bAutoStart && NewAction->CanStart(Instigator))
		{
			NewAction->StartAction(Instigator);
		}
	}
}

void USActionComponent::RemoveAction(USAction* ActionToRemove)
{
	if (!ensure(ActionToRemove && !ActionToRemove->IsRunning()))
	{
		return;
	}
	Actions.Remove(ActionToRemove);
}

bool USActionComponent::StartActionByName(AActor* InstigatorActor, FName ActionName)
{
	SCOPE_CYCLE_COUNTER(STAT_StartActionByName);
	for (USAction* Action : Actions)
	{
		if (Action && Action->GetActionName() == ActionName)
		{
			if (!Action->CanStart(InstigatorActor))
			{
				FString FailedMsg = FString::Printf(TEXT("Failed to run %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FailedMsg);
				return false;
			}
			if (!GetOwner()->HasAuthority())
			{
				ServerStartAction(InstigatorActor, ActionName);
			}
			TRACE_BOOKMARK(TEXT("StartActionByName: %s"), ActionName);
			
			Action->StartAction(InstigatorActor);
			return true;
		}
	}
	return false;
}

bool USActionComponent::StopActionByName(AActor* InstigatorActor, FName ActionName)
{
	for (USAction* Action: Actions)
	{
		if (Action && Action->GetActionName() == ActionName)
		{
			if (Action->IsRunning())
			{
				if (!GetOwner()->HasAuthority())
				{
					ServerStopAction(InstigatorActor, ActionName);
				}
				Action->StopAction(InstigatorActor);
                return true;
			}
		}
	}
	return false;
}

void USActionComponent::ServerStartAction_Implementation(AActor* InstigatorActor, FName ActionName)
{
	StartActionByName(InstigatorActor, ActionName);
}

void USActionComponent::ServerStopAction_Implementation(AActor* InstigatorActor, FName ActionName)
{
	StopActionByName(InstigatorActor, ActionName);
}

void USActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USActionComponent, Actions);
}

bool USActionComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething =  Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (USAction* Action : Actions)
	{
		WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
	}
	return WroteSomething;
}
