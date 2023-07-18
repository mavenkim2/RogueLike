// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKE_API ASPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category="HUD")
	TSubclassOf<UUserWidget> HUDClass;
	
	UUserWidget* MainHUD;
	
	virtual void BeginPlay() override;

public:
	void ReconstructHUD();
	
};
