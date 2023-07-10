// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectileBaseClass.generated.h"
class USoundCue;
class UProjectileMovementComponent;
class USphereComponent;
class UParticleSystemComponent;

UCLASS(Abstract)
class ROGUELIKE_API ASProjectileBaseClass : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASProjectileBaseClass();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effects")
	UParticleSystem* ParticleTemplate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Effects")
	UParticleSystemComponent* ParticleSystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Audio")
	UAudioComponent* FlightSoundComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Audio")
	USoundCue* ImpactSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	TSubclassOf<UCameraShakeBase> CameraShake;

	UPROPERTY(EditDefaultsOnly, Category="Effects|Shake")
	float CameraShakeInnerRadius;

	UPROPERTY(EditDefaultsOnly, Category="Effects|Shake")
	float CameraShakeOuterRadius;

	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode();

	virtual void PostInitializeComponents() override;
	
};
