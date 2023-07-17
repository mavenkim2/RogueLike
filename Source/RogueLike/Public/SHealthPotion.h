// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "SPowerup.h"
#include "GameFramework/Actor.h"
#include "SHealthPotion.generated.h"

UCLASS()
class ROGUELIKE_API ASHealthPotion : public ASPowerup
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASHealthPotion();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Health")
	float HealAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Credits")
	int32 CreditsCost;

public:	
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};
