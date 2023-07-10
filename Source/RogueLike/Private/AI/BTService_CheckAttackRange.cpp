// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_CheckAttackRange::UBTService_CheckAttackRange()
{
	MaxAttackRange = 2000.f;
}


void UBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent(); ensure(BlackboardComponent))
	{
		AAIController* AIController = OwnerComp.GetAIOwner();
		AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject("TargetActor"));
		if (AIController && TargetActor)
		{
			APawn* AIPawn = AIController->GetPawn();
			if (ensure(AIPawn))
			{
				float Distance = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());
				bool bIsWithinAttackRange = Distance < MaxAttackRange;
				bool bHasLineOfSight = false;
				if (bIsWithinAttackRange)
				{
					bHasLineOfSight = AIController->LineOfSightTo(TargetActor);
				}
				BlackboardComponent->SetValueAsBool(WithinAttackRange.SelectedKeyName, (bIsWithinAttackRange && bHasLineOfSight));
			}
		}
	}
}

