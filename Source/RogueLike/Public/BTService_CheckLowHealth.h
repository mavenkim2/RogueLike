// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckLowHealth.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKE_API UBTService_CheckLowHealth : public UBTService
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category="AI")
	FBlackboardKeySelector IsLowHealth;

	UPROPERTY(EditAnywhere, Category="AI")
	float HealthThresholdFraction;
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	UBTService_CheckLowHealth();
};
