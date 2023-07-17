// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include "EngineUtils.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SPlayerState.h"
#include "SPowerup.h"
#include "AI/SAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("rg.SpawnBots"), true, TEXT("Enable spawning of bots via timer"), ECVF_Cheat);

ASGameModeBase::ASGameModeBase()
{
	CreditsOnKill = 10;
	PlayerStateClass = ASPlayerState::StaticClass();
	DesiredPowerupCount = 10;
	MinimumPowerupSpawnDistance = 500.f;
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASGameModeBase::SpawnBotTimerElapsed, 2.0f, true);

	if (PowerupClasses.Num() > 0)
	{
		UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnPowerUpsEnvQuery, this, EEnvQueryRunMode::AllMatching, nullptr);
        QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnPowerUpSpawnQueryCompleted);
	}
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
	
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotsEnvQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnSpawnBotsQueryCompleted);
	}
}

void ASGameModeBase::OnSpawnBotsQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
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

void ASGameModeBase::OnPowerUpSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	// spawn up to a specified number of powerups
	// randomly select a location from the eqs, then remove it so it can't be reselected
	// ensure that the location selected is a minimum distance away from all spawned powerups
	// randomly select a powerup from a powerup class list to spawn
	// spawn

	if (QueryStatus != EEnvQueryStatus::Success)
	{
		return;
	}
	TArray<FVector> SpawnLocations;
	if (!QueryInstance->GetQueryResultsAsLocations(SpawnLocations))
	{
		return;
	}
	TArray<FVector> UsedLocations;
	int32 PowerupCount = 0;
	while (PowerupCount < DesiredPowerupCount)
	{
		int Index = FMath::RandRange(0, SpawnLocations.Num() - 1);
		FVector SpawnLocation = SpawnLocations[Index];
		SpawnLocations.RemoveAt(Index);
		bool bIsTooClose = false;
		for (FVector Location : UsedLocations)
		{
			float Distance = FVector::Distance(SpawnLocation, Location);
			if (Distance < MinimumPowerupSpawnDistance)
			{
				bIsTooClose = true;
				break;
			}
		}
		if (bIsTooClose)
		{
			continue;
		}
		int PowerupChoice = FMath::RandRange(0, PowerupClasses.Num() - 1);
		GetWorld()->SpawnActor<AActor>(PowerupClasses[PowerupChoice], SpawnLocation, FRotator::ZeroRotator);
		PowerupCount++;
		UsedLocations.Add(SpawnLocation);
	}
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
	// wipe all statuses on dead actor?
	if (ASCharacter* Player = Cast<ASCharacter>(VictimActor))
	{
		FTimerHandle TimerHandle_RespawnDelay;
		FTimerDelegate RespawnDelegate;
		RespawnDelegate.BindUFunction(this, "RespawnPlayedKilled", Player->GetController());
		float RespawnDelay = 2.f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, RespawnDelegate, RespawnDelay, false);
	}
	else if (ACharacter* Character = Cast<ACharacter>(InstigatorActor))
	{
		if (ASPlayerState* PlayerState = Character->GetPlayerState<ASPlayerState>())
		{
			PlayerState->ApplyCreditsChange(CreditsOnKill);
		}
	}
	UE_LOG(LogTemp, Log, TEXT("Actor %s killed by %s"), *GetNameSafe(VictimActor), *GetNameSafe(InstigatorActor));
}