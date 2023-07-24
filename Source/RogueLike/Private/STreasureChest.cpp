// Fill out your copyright notice in the Description page of Project Settings.


#include "STreasureChest.h"

#include "Net/UnrealNetwork.h"

// Sets default values
ASTreasureChest::ASTreasureChest()
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("Base Mesh");
	SetRootComponent(BaseMesh);

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("Lid Mesh");
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 110.f;

	bReplicates = true;
}

void ASTreasureChest::OnRep_OpenLid()
{
	float Pitch = bIsLidOpened ? TargetPitch : 0.f;
	LidMesh->SetRelativeRotation(FRotator(Pitch, 0.f, 0.f));
}


void ASTreasureChest::Interact_Implementation(APawn* InstigatorPawn)
{
	bIsLidOpened = !bIsLidOpened;
	OnRep_OpenLid();
}

void ASTreasureChest::OnActorLoaded_Implementation()
{
	OnRep_OpenLid();
}

void ASTreasureChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASTreasureChest, bIsLidOpened);
}

