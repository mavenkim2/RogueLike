// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckLowHealth.h"

#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_CheckLowHealth::UBTService_CheckLowHealth()
{
	HealthThresholdFraction = .2f;
}

void UBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ensure(AIPawn))
	{
		USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(AIPawn);
		if (ensure(AttributeComponent))
		{
			bool bLowHealth = (AttributeComponent->GetHealth() / AttributeComponent->GetMaxHealth()) < HealthThresholdFraction;
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(IsLowHealth.SelectedKeyName, bLowHealth);
		}
	}
}
