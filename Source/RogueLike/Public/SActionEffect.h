// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SActionEffect.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKE_API USActionEffect : public USAction
{
	GENERATED_BODY()

public:
	USActionEffect();

	virtual void StartAction_Implementation(AActor* InstigatorActor) override;
	virtual void StopAction_Implementation(AActor* InstigatorActor) override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Effects")
	float Duration;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Effects")
	float Period;
	
	FTimerHandle DurationTimer;
	FTimerHandle PeriodTimer;

	UFUNCTION(BlueprintNativeEvent, Category = "Effects")
	void ExecutePeriodicEffect(AActor* Instigator);
};
