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
	void PostInitializeComponents();
	virtual void BeginPlay() override;

protected:
	FTimerHandle TimerHandleDash;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UParticleSystem* ParticleTemplate;

	UFUNCTION()
	void Detonate();

	UFUNCTION()
	void DetonateCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void Teleport();
};
