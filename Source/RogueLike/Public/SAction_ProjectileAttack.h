// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SAction_ProjectileAttack.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKE_API USAction_ProjectileAttack : public USAction
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(VisibleAnywhere, Category="Effects")
	FName HandSocketName;

	UPROPERTY(EditAnywhere, Category="Attack")
	UAnimMontage* AttackAnimation;

	UPROPERTY(EditDefaultsOnly, Category="Attack")
	float AttackAnimationDelay;

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	UParticleSystem* CastingEffect;

	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* Instigator);

public:
	virtual void StartAction_Implementation(AActor* InstigatorActor) override;
	USAction_ProjectileAttack();
};
