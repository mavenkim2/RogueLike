// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SAction.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, Abstract)
class ROGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent,Category="Action")
	void StartAction(AActor* InstigatorActor);
	
	UFUNCTION(BlueprintNativeEvent, Category="Action")
	void StopAction(AActor* InstigatorActor);

	FName GetActionName() const;

	UWorld* GetWorld() const override;

protected:
	UPROPERTY(EditDefaultsOnly, Category="Action")
	FName ActionName;
};
