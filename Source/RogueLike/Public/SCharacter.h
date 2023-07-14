// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "SProjectileBaseClass.h"
#include "SCharacter.generated.h"

class USActionComponent;
class USAttributeComponent;
class USInteractionComponent;
class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class ROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Components
	UPROPERTY(VisibleAnywhere, Category="Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, Category="Components")
	UCameraComponent* CameraComponent;
	
	UPROPERTY(VisibleAnywhere, Category="Components")
	USInteractionComponent* InteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USActionComponent* ActionComponent;

	// Effects
	UPROPERTY(VisibleAnywhere, Category="Effects")
	FName TimeToHitParamName;
	
	// Inputs
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputMappingContext* MappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* PrimaryAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* PrimaryInteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* TeleportAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* BlackholeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* SprintAction;

	// Movement
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);

	// Abilities
	void PrimaryAttack();
	void PrimaryInteract();
	void Dash();
	void Blackhole();
	void SprintStart();
	void SprintStop();

	//Exec
	UFUNCTION(Exec)
	void HealSelf(float HealAmount = 100);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);
	
	FVector GetPawnViewLocation() const;
};
