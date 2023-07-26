// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionEffect.h"

#include "SActionComponent.h"
#include "GameFramework/GameStateBase.h"

// duration period, each with timers
// start action starts the timers, stop action clears and stops the timers
// duration is the total time the action persists, period is the time for each tick
// start action automatically, remove action automatically when duration elapsed
USActionEffect::USActionEffect()
{
	bAutoStart = true;
}

void USActionEffect::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);

	if (Duration > 0.f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAction", InstigatorActor);
		GetWorld()->GetTimerManager().SetTimer(DurationTimer, Delegate, Duration, false);
	}
	if (Period > 0.f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodicEffect", InstigatorActor);
		GetWorld()->GetTimerManager().SetTimer(PeriodTimer, Delegate, Period, true);
	}
}

void USActionEffect::StopAction_Implementation(AActor* InstigatorActor)
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if (TimerManager.GetTimerRemaining(PeriodTimer) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(InstigatorActor);
	}
	
	Super::StopAction_Implementation(InstigatorActor);

	TimerManager.ClearTimer(DurationTimer);
	TimerManager.ClearTimer(PeriodTimer);

	if(USActionComponent* ActionComponent = Cast<USActionComponent>(GetOuter()))
	{
		ActionComponent->RemoveAction(this);
	}
}

float USActionEffect::GetTimeElapsed() const
{
	AGameStateBase* GameStateBase = GetWorld()->GetGameState();
	if (GameStateBase)
	{
		return GameStateBase->GetServerWorldTimeSeconds() - TimeStarted;
	}
	return 0.f;
}

void USActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{
}
