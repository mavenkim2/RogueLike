// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTRangedAttack.h"

#include "AIController.h"
#include "SAttributeComponent.h"
#include "SProjectileBaseClass.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

USBTTRangedAttack::USBTTRangedAttack()
{
	MaxVerticalBulletSpread = 2.f;
	MaxHorizontalBulletSpread = 2.f;
}

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
		if (!USAttributeComponent::IsActorAlive(TargetActor))
		{
			return EBTNodeResult::Failed;
		}
		FVector MuzzleLocation = AICharacter->GetMesh()->GetSocketLocation("Muzzle_01");
		FRotator MuzzleRotation = (TargetActor->GetActorLocation() - MuzzleLocation).Rotation();
		MuzzleRotation.Pitch += FMath::RandRange(0.f, MaxVerticalBulletSpread);;
		MuzzleRotation.Yaw += FMath::RandRange(-MaxHorizontalBulletSpread, MaxHorizontalBulletSpread);;
		
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParameters.Instigator = AICharacter;
		
		AActor* NewProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParameters);
		return NewProjectile ? EBTNodeResult::Succeeded :EBTNodeResult::Failed;
	}
	return EBTNodeResult::Failed;
}


