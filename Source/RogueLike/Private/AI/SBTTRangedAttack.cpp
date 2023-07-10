// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTRangedAttack.h"

#include "AIController.h"
#include "SProjectileBaseClass.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type USBTTRangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIController* AIController = OwnerComp.GetAIOwner(); ensure(AIController))
	{
		ACharacter* AICharacter = Cast<ACharacter>(AIController->GetPawn());
		if (!AICharacter)
		{
			return EBTNodeResult::Failed;
		}
		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		if (!TargetActor)
		{
			return EBTNodeResult::Failed;
		}
		FVector SpawnLocation = AICharacter->GetMesh()->GetSocketLocation("Muzzle_01");
		FRotator LookAtRotation = (TargetActor->GetActorLocation() - SpawnLocation).Rotation();
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		AActor* NewProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLocation, LookAtRotation, SpawnParameters);
		return NewProjectile ? EBTNodeResult::Succeeded :EBTNodeResult::Failed;
	}
	return EBTNodeResult::Failed;
}
