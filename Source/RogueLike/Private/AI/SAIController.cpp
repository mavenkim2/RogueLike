// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void ASAIController::BeginPlay()
{
	Super::BeginPlay();
	if (ensureMsgf(BehaviorTree, TEXT("Behavior tree is null! Make sure to assign behavior tree in blueprint.")))
	{
		RunBehaviorTree(BehaviorTree);
	}
	// if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0))
	// {
	// 	GetBlackboardComponent()->SetValueAsObject("TargetActor", PlayerPawn);
	// }
}
