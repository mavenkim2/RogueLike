// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBaseClass.h"
#include "GameFramework/Actor.h"
#include "SDashProjectile.generated.h"

UCLASS()
class ROGUELIKE_API ASDashProjectile : public ASProjectileBaseClass
{
	GENERATED_BODY()

public:
	ASDashProjectile();
	virtual void BeginPlay() override;

protected:
	FTimerHandle TimerHandleDash;
	
	virtual void Explode_Implementation() override;
	
	void Teleport();
	
	virtual void PostInitializeComponents() override;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float TeleportDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float DetonateDelay;
};
