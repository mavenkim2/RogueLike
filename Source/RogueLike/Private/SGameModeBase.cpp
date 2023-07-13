// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include "EngineUtils.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "AI/SAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("rg.SpawnBots"), true, TEXT("Enable spawning of bots via timer"), ECVF_Cheat);

ASGameModeBase::ASGameModeBase()
{
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASGameModeBase::SpawnBotTimerElapsed, 2.0f, true);
}

void ASGameModeBase::KillAll()
{
	for (ASAICharacter* AICharacter : TActorRange<ASAICharacter>(GetWorld()))
	{
		USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(AICharacter);
		if (ensure(AttributeComponent) && AttributeComponent->IsAlive())
		{
			AttributeComponent->Kill(this);
		}
	}
}

void ASGameModeBase::SpawnBotTimerElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot spawning disabled via cvar 'CVarSpawnBots'"));
		return;
	}
	int32 AliveMinions = 0;
	for (ASAICharacter* AICharacter : TActorRange<ASAICharacter>(GetWorld()))
	{
		USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(AICharacter);
		if (ensure(AttributeComponent) && AttributeComponent->IsAlive())
		{
			AliveMinions += 1;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Found %i alive bots"), AliveMinions);
	float MaxMinions = 10.f;
	if (CurveFloat)
	{
		MaxMinions = CurveFloat->GetFloatValue(GetWorld()->GetTimeSeconds());
	}
	if (AliveMinions >= MaxMinions)
	{
		UE_LOG(LogTemp, Log, TEXT("At maximum capacity, skipping spawn."));
		return;
	}
	
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, EnvQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryCompleted);
	}
}

void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		return;
	}
	TArray<FVector> SpawnLocations;
	if(!QueryInstance->GetQueryResultsAsLocations(SpawnLocations))
	{
		return;
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<AActor>(MinionClass, SpawnLocations[0], FRotator::ZeroRotator, SpawnParameters);
	DrawDebugSphere(GetWorld(), SpawnLocations[0], 60.f, 20, FColor::Blue, false, 30.f);
}

void ASGameModeBase::RespawnPlayedKilled(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();
		RestartPlayer(Controller);
	}
}

void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* InstigatorActor)
{
	ASCharacter* Player = Cast<ASCharacter>(VictimActor);
	if (Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;
		FTimerDelegate RespawnDelegate;
		RespawnDelegate.BindUFunction(this, "RespawnPlayedKilled", Player->GetController());
		float RespawnDelay = 2.f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, RespawnDelegate, RespawnDelay, false);
	}
	UE_LOG(LogTemp, Log, TEXT("Actor %s killed by %s"), *GetNameSafe(VictimActor), *GetNameSafe(InstigatorActor));
}