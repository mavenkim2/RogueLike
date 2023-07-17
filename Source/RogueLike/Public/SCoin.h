// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerup.h"
#include "SCoin.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKE_API ASCoin : public ASPowerup
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Credits")
	int32 CreditsRewarded;

public:
	ASCoin();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};
