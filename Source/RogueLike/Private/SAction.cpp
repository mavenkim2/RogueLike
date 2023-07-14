// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"

void USAction::StartAction_Implementation(AActor* InstigatorActor)
{
	UE_LOG(LogTemp, Log, TEXT("%s action started"), *GetNameSafe(this))
}

void USAction::StopAction_Implementation(AActor* InstigatorActor)
{
	UE_LOG(LogTemp, Log, TEXT("%s action stopped"), *GetNameSafe(this))
}

FName USAction::GetActionName() const
{
	return ActionName;
}

UWorld* USAction::GetWorld() const
{
	UActorComponent* ActorComponent = Cast<UActorComponent>(GetOuter());
	if (ActorComponent)
	{
		return ActorComponent->GetWorld();
	}
	return nullptr;
}
