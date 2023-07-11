// Fill out your copyright notice in the Description page of Project Settings.


#include "STreasureChest.h"

// Sets default values
ASTreasureChest::ASTreasureChest()
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("Base Mesh");
	SetRootComponent(BaseMesh);

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("Lid Mesh");
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 110.f;
}


void ASTreasureChest::Interact_Implementation(APawn* InstigatorPawn)
{
	LidMesh->SetRelativeRotation(FRotator(TargetPitch, 0.f, 0.f));
}

