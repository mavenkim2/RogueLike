// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"

#include "SActionComponent.h"
#include "Net/UnrealNetwork.h"
#include "RogueLike/RogueLike.h"

bool USAction::CanStart_Implementation(AActor* InstigatorActor)
{
	if (bIsRunning || bOnCooldown)
	{
		return false;
	}
	USActionComponent* ActionComponent = GetOwningComponent();
	if (ActionComponent->GetActiveGameplayTags().HasAny(BlockedTags))
	{
		return false;
	}
	return true;
}

void USAction::StartAction_Implementation(AActor* InstigatorActor)
{
	// UE_LOG(LogTemp, Log, TEXT("%s action started"), *GetNameSafe(this));
	LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);
	USActionComponent* ActionComponent = GetOwningComponent();
	ActionComponent->GetActiveGameplayTags().AppendTags(GrantsTags);
	bIsRunning = true;
}

void USAction::StopAction_Implementation(AActor* InstigatorActor)
{
	if (!bIsRunning)
	{
		return;
	}
	// UE_LOG(LogTemp, Log, TEXT("%s action stopped"), *GetNameSafe(this))
	LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::White);
	USActionComponent* ActionComponent = GetOwningComponent();
	ActionComponent->GetActiveGameplayTags().RemoveTags(GrantsTags);
	bIsRunning = false;
	StartCooldown();
}


bool USAction::IsRunning() const
{
	return bIsRunning;
}

void USAction::StartCooldown()
{
	if (TimeOnCooldown == 0.f)
	{
		return;
	}
	bOnCooldown = true;
	FTimerHandle CooldownTimer;
	GetWorld()->GetTimerManager().SetTimer(CooldownTimer, this, &USAction::StopCooldown, TimeOnCooldown);
}

void USAction::StopCooldown()
{
	bOnCooldown = false;
}

FName USAction::GetActionName() const
{
	return ActionName;
}

UWorld* USAction::GetWorld() const
{
	UActorComponent* ActorComponent = GetOwningComponent();
	if (ActorComponent)
	{
		return ActorComponent->GetWorld();
	}
	return nullptr;
}

USActionComponent* USAction::GetOwningComponent() const
{
	return Cast<USActionComponent>(GetOuter());
}

void USAction::OnRep_IsRunning()
{
	if (bIsRunning)
	{
		StartAction(nullptr);
	}
	else
	{
		StopAction(nullptr);
	}
}

void USAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAction, bIsRunning);
} 
