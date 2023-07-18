// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerController.h"

#include "Blueprint/UserWidget.h"

void ASPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (ensure(HUDClass))
	{
		if (IsLocalPlayerController())
		{
			MainHUD = CreateWidget<UUserWidget>(this, HUDClass);
			MainHUD->AddToViewport();
		}
	}
}

void ASPlayerController::ReconstructHUD()
{
	if (MainHUD)
	{
		if (IsLocalPlayerController())
		{
			MainHUD->RemoveFromParent();
			MainHUD = CreateWidget<UUserWidget>(this, HUDClass);
			MainHUD->AddToViewport();
		}
	}
}
