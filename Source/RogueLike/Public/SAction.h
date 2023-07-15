// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
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
	bool CanStart(AActor* InstigatorActor);
	
	UFUNCTION(BlueprintNativeEvent,Category="Action")
	void StartAction(AActor* InstigatorActor);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Action")
	void StopAction(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable, Category="Action")
	bool IsRunning() const;
	
	void StartCooldown();
	
	UFUNCTION()
	void StopCooldown();
	
	FName GetActionName() const;

	UWorld* GetWorld() const override;

protected:
	UPROPERTY(EditDefaultsOnly, Category="Action")
	FName ActionName;

	UPROPERTY(EditDefaultsOnly, Category="Tags")
	FGameplayTagContainer BlockedTags;
	
	UPROPERTY(EditDefaultsOnly, Category="Tags")
	FGameplayTagContainer GrantsTags;

	bool bIsRunning;

	bool bOnCooldown;

	UPROPERTY(EditDefaultsOnly, Category="Action")
	float TimeOnCooldown;
};
