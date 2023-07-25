// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include "EngineUtils.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SPlayerController.h"
#include "SPlayerState.h"
#include "SPowerup.h"
#include "SSaveGame.h"
#include "AI/SAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("rg.SpawnBots"), true, TEXT("Enable spawning of bots via timer"), ECVF_Cheat);

ASGameModeBase::ASGameModeBase()
{
	CreditsOnKill = 10;
	PlayerStateClass = ASPlayerState::StaticClass();
	DesiredPowerupCount = 10;
	MinimumPowerupSpawnDistance = 500.f;
	SlotName = "SaveGame01";
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASGameModeBase::SpawnBotTimerElapsed, 5.f, true);

	if (PowerupClasses.Num() > 0)
	{
		UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnPowerUpsEnvQuery, this, EEnvQueryRunMode::AllMatching, nullptr);
        QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnPowerUpSpawnQueryCompleted);
	}
}

void ASGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	LoadSaveGame();
}

void ASGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	ASPlayerState* PlayerState = NewPlayer->GetPlayerState<ASPlayerState>();
	if (PlayerState)
	{
		PlayerState->LoadPlayerState(CurrentSaveGame);
	}
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
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

void ASGameModeBase::RespawnPlayedKilled(ASPlayerController* Controller)
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
		RespawnDelegate.BindUFunction(this, "RespawnPlayedKilled", Player->GetController<ASPlayerController>());
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

void ASGameModeBase::WriteSaveGame()
{
	for (int i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		ASPlayerState* PlayerState = Cast<ASPlayerState>(GameState->PlayerArray[i]);
		if (PlayerState)
		{
			PlayerState->SavePlayerState(CurrentSaveGame);
			break;
		}
	}
	CurrentSaveGame->ActorSaveData.Empty();
	for (AActor* Actor : TActorRange<AActor>(GetWorld()))
	{
		if (Actor->Implements<USGameplayInterface>())
		{
			FActorSaveData SaveData = {Actor->GetName(), Actor->GetTransform()};
			FMemoryWriter MemoryWriter(SaveData.ByteArray);
			FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
			Archive.ArIsSaveGame = true;
			Actor->Serialize(Archive);
			CurrentSaveGame->ActorSaveData.Add(SaveData);
		}
	}
	
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName.ToString(), 0);
}

void ASGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName.ToString(), 0))
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName.ToString(), 0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Log, TEXT("Failed to load save game data"));
			return;
		}
		for (AActor* Actor : TActorRange<AActor>(GetWorld()))
		{
			if (Actor->Implements<USGameplayInterface>())
			{
				for (FActorSaveData SaveData : CurrentSaveGame->ActorSaveData)
				{
					if (SaveData.Name == Actor->GetName())
					{
						Actor->SetActorTransform(SaveData.Transform);
						FMemoryReader MemoryReader(SaveData.ByteArray);
						FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
						Archive.ArIsSaveGame = true;
						Actor->Serialize(Archive);
						ISGameplayInterface::Execute_OnActorLoaded(Actor);
						break;
					}
				}
			}
		}
		UE_LOG(LogTemp, Log, TEXT("Loaded save game data"));
	}
	else
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::CreateSaveGameObject(USSaveGame::StaticClass()));
		UE_LOG(LogTemp, Log, TEXT("Created save game data"));
	}
}