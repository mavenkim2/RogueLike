// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SHealthPotion.generated.h"

UCLASS()
class ROGUELIKE_API ASHealthPotion : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASHealthPotion();

protected:

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float HealAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Cooldown;

	FTimerHandle CooldownTimer;

	bool bIsActive;

public:	
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	UFUNCTION()
	void Activate();
	void Deactivate();
};
