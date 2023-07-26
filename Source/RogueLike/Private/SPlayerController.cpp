// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerController.h"

#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerInput.h"

void ASPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	OnPawnChanged.Broadcast(InPawn);
}

void ASPlayerController::BeginPlayingState()
{
	BlueprintBeginPlayingState();
}

void ASPlayerController::TogglePauseMenu()
{
	if (PauseMenuWidget)
	{
		PauseMenuWidget->RemoveFromParent();
		PauseMenuWidget = nullptr;
		SetShowMouseCursor(false);
		SetInputMode(FInputModeGameOnly());
		return;
	}
	if (PauseMenuWidgetClass && !PauseMenuWidget)
	{
		PauseMenuWidget = CreateWidget<UUserWidget>(this, PauseMenuWidgetClass);
		if (PauseMenuWidget)
		{
			PauseMenuWidget->AddToViewport(100);
			SetShowMouseCursor(true);
			SetInputMode(FInputModeUIOnly());
			PlayerInput->FlushPressedKeys();
		}
	}
}

void ASPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(TogglePauseMenuInputAction, ETriggerEvent::Triggered, this, &ASPlayerController::TogglePauseMenu);
	}
}
