// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "SActionComponent.generated.h"


class USAction;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROGUELIKE_API USActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Action")
	void AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass);
	
	UFUNCTION(BlueprintCallable, Category="Action")
	void RemoveAction(USAction* ActionToRemove);

	UFUNCTION(BlueprintCallable, Category="Action")
	bool StartActionByName(AActor* InstigatorActor, FName ActionName);

	UFUNCTION(BlueprintCallable, Category="Action")
	bool StopActionByName(AActor* InstigatorActor, FName ActionName);

	UFUNCTION(Server, Reliable)
	void ServerStartAction(AActor* InstigatorActor, FName ActionName);
	
	UFUNCTION(Server, Reliable)
	void ServerStopAction(AActor* InstigatorActor, FName ActionName);

	UFUNCTION(BlueprintCallable, Category="Tags")
	FGameplayTagContainer& GetActiveGameplayTags();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category="Action")
	USAction* GetAction(TSubclassOf<USAction> ActionClass);

	bool ReplicateSubobjects(UActorChannel *Channel, FOutBunch *Bunch, FReplicationFlags *RepFlags) override;

	USActionComponent();

protected:
	UPROPERTY(EditAnywhere, Category="Actions")
	TArray<TSubclassOf<USAction>> DefaultActions;
	
	UPROPERTY(Replicated)
	TArray<USAction*> Actions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Actions")
	FGameplayTagContainer ActiveGameplayTags;
	
	virtual void BeginPlay() override;

};
