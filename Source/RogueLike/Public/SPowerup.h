// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SPowerup.generated.h"

UCLASS()
class ROGUELIKE_API ASPowerup : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPowerup();

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Cooldown")
	float RespawnTime;
	
	UFUNCTION()
	void Activate();
	void DeactivateAndCooldown();

	UPROPERTY(ReplicatedUsing="OnRep_SetState")
	bool bIsActive;

	UFUNCTION()
	void OnRep_SetState();

public:	
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	virtual FText GetInteractText_Implementation(APawn* InstigatorPawn) override;
};
