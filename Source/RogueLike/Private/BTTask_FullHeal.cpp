// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FullHeal.h"

#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_FullHeal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!AIPawn)
	{
		return EBTNodeResult::Failed;
	}
	USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(AIPawn);
	if (ensure(AttributeComponent))
	{
		AttributeComponent->ApplyHealthChange(AIPawn, AttributeComponent->GetMaxHealth());
	}
	return EBTNodeResult::Succeeded;
}
