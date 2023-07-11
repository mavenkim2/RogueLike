// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"

#include "BrainComponent.h"
#include "SAttributeComponent.h"
#include "AI/SAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("Pawn Sensing Component");
	AttributeComponent = CreateDefaultSubobject<USAttributeComponent>("Attribute Component");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	TimeToHitParamName = "TimeToHit";
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
	AttributeComponent->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
}

void ASAICharacter::SetTargetActor(AActor* NewTarget)
{
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		AIController->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
	}
}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);
}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.f)
	{
		if (InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}
		
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->GetTimeSeconds());
		
		if (NewHealth <= 0.0f)
		{
			AAIController* AIController = Cast<AAIController>(GetController());
			if (AIController)
			{
				AIController->GetBrainComponent()->StopLogic("Killed");
			}
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");
			SetLifeSpan(10.f);
		}
	}
}
