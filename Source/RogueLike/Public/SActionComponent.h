// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SActionComponent.generated.h"


class USAction;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROGUELIKE_API USActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Action")
	void AddAction(TSubclassOf<USAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category="Action")
	bool StartActionByName(AActor* InstigatorActor, FName ActionName);

	UFUNCTION(BlueprintCallable, Category="Action")
	bool StopActionByName(AActor* InstigatorActor, FName ActionName);
	
	USActionComponent();

protected:
	UPROPERTY(EditAnywhere, Category="Actions")
	TArray<TSubclassOf<USAction>> DefaultActions;
	
	UPROPERTY(BlueprintReadOnly)
	TMap<FName, USAction*> Actions;
	
	virtual void BeginPlay() override;
};
