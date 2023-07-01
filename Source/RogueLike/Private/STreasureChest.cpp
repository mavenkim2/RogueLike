// Fill out your copyright notice in the Description page of Project Settings.


#include "STreasureChest.h"

// Sets default values
ASTreasureChest::ASTreasureChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("Base Mesh");
	SetRootComponent(BaseMesh);

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("Lid Mesh");
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 110.f;
}

// Called when the game starts or when spawned
void ASTreasureChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASTreasureChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASTreasureChest::Interact_Implementation(APawn* InstigatorPawn)
{
	LidMesh->SetRelativeRotation(FRotator(TargetPitch, 0.f, 0.f));
}

