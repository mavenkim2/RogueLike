// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"

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
	UEnvQuery* EnvQuery;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	TSubclassOf<AActor> MinionClass;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	UCurveFloat* CurveFloat;
	
public:
	ASGameModeBase();
	virtual void StartPlay() override;

	UFUNCTION(Exec)
	void KillAll();

	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void RespawnPlayedKilled(AController* Controller);
	
	void OnActorKilled(AActor* VictimActor, AActor* InstigatorActor);
};
