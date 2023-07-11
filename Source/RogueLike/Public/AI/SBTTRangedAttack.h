// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SBTTRangedAttack.generated.h"

class ASProjectileBaseClass;
/**
 * 
 */
UCLASS()
class ROGUELIKE_API USBTTRangedAttack : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:

	USBTTRangedAttack();
protected:
	UPROPERTY(EditAnywhere, Category="AI")
	TSubclassOf<ASProjectileBaseClass> ProjectileClass;

	UPROPERTY(EditAnywhere, Category="AI")
	float MaxVerticalBulletSpread;

	UPROPERTY(EditAnywhere, Category="AI")
	float MaxHorizontalBulletSpread;
};
