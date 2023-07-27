// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SAnimInstance.generated.h"

class USActionComponent;
/**
 * 
 */
UCLASS()
class ROGUELIKE_API USAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category="Animation")
	USActionComponent* ActionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Animation")
	bool bIsStunned;
	
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
