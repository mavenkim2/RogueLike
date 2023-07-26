// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SPlayerController.generated.h"

class UInputAction;
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnChanged, APawn*, InPawn);

UCLASS()
class ROGUELIKE_API ASPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category="UI")
	UInputAction* TogglePauseMenuInputAction;
	
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UUserWidget> PauseMenuWidgetClass;

	UUserWidget* PauseMenuWidget;
public:
	UPROPERTY(BlueprintAssignable)
	FOnPawnChanged OnPawnChanged;
	
	virtual void SetPawn(APawn* InPawn) override;

	virtual void BeginPlayingState() override;

	UFUNCTION(BlueprintImplementableEvent)
	void BlueprintBeginPlayingState();

	UFUNCTION(BlueprintCallable, Category="UI")
	void TogglePauseMenu();

	virtual void SetupInputComponent() override;
};
