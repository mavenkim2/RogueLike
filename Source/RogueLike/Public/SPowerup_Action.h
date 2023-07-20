// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerup.h"
#include "SPowerup_Action.generated.h"

class USAction;
/**
 * 
 */
UCLASS()
class ROGUELIKE_API ASPowerup_Action : public ASPowerup
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category="Powerup")
	TSubclassOf<USAction> ActionToGrant;
	
public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};
