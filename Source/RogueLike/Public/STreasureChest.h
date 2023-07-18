// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "STreasureChest.generated.h"

UCLASS()
class ROGUELIKE_API ASTreasureChest : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASTreasureChest();

	UPROPERTY(EditAnywhere)
	float TargetPitch;

protected:
	UPROPERTY(ReplicatedUsing="OnRep_OpenLid", BlueprintReadOnly)
	bool bIsLidOpened;

	UFUNCTION()
	void OnRep_OpenLid();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;

public:	

	void Interact_Implementation(APawn* InstigatorPawn) override;
};
