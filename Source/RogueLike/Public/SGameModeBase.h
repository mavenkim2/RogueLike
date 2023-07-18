// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPlayerController.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"

class ASPowerup;
class UEnvQuery;
/**
 * 
 */
UCLASS()
class ROGUELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	FTimerHandle SpawnTimer;
	
	UPROPERTY(EditDefaultsOnly, Category="AI")
	UEnvQuery* SpawnBotsEnvQuery;
	
	UPROPERTY(EditDefaultsOnly, Category="PowerUp")
	UEnvQuery* SpawnPowerUpsEnvQuery;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	TSubclassOf<AActor> MinionClass;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	UCurveFloat* CurveFloat;

	UPROPERTY(EditDefaultsOnly, Category="Credits")
	int32 CreditsOnKill;

	UPROPERTY(EditDefaultsOnly, Category="PowerUp")
	TArray<TSubclassOf<ASPowerup>> PowerupClasses;

	UPROPERTY(EditDefaultsOnly, Category="PowerUp")
	int32 DesiredPowerupCount;

	UPROPERTY(EditDefaultsOnly, Category="PowerUp")
	float MinimumPowerupSpawnDistance;
	
public:
	ASGameModeBase();
	virtual void StartPlay() override;

	UFUNCTION(Exec)
	void KillAll();

	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void OnSpawnBotsQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	
	UFUNCTION()
	void OnPowerUpSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void RespawnPlayedKilled(ASPlayerController* Controller);
	
	void OnActorKilled(AActor* VictimActor, AActor* InstigatorActor);
};
