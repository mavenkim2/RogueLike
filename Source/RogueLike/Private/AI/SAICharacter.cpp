// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"

#include "BrainComponent.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SWorldUserWidget.h"
#include "AI/SAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("Pawn Sensing Component");
	AttributeComponent = CreateDefaultSubobject<USAttributeComponent>("Attribute Component");
	ActionComponent = CreateDefaultSubobject<USActionComponent>("Action Component");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	TimeToHitParamName = "TimeToHit";
	TargetActorKey = "TargetActor";

	// GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
	AttributeComponent->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
}

AActor* ASAICharacter::GetTargetActor() const
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		return Cast<AActor>(AIC->GetBlackboardComponent()->GetValueAsObject(TargetActorKey));
	}
	return nullptr;
}

void ASAICharacter::SetTargetActor(AActor* NewTarget)
{
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		AIController->GetBlackboardComponent()->SetValueAsObject(TargetActorKey, NewTarget);
	}
}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	if (GetTargetActor() != Pawn)
	{
		SetTargetActor(Pawn);
		MulticastOnPawnSeen();
	}
}

void ASAICharacter::MulticastOnPawnSeen_Implementation()
{
	USWorldUserWidget* Widget = CreateWidget<USWorldUserWidget>(GetWorld(), SpottedWidgetClass);
	if (Widget)
	{
		Widget->AttachedActor = this;
		Widget->AddToViewport(10);
	}
}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.f)
	{
		if (InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}

		if (!ActiveHealthBar && NewHealth > 0.f)
		{
			ActiveHealthBar = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBar)
			{
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}
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
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			SetLifeSpan(10.f);
		}
	}
}
