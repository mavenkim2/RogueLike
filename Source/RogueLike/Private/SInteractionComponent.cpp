// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"

#include "SGameplayInterface.h"
#include "SWorldUserWidget.h"
#include "Blueprint/UserWidget.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("rg.DebugDraw"), false, TEXT("Enable Debug lines for Interact Component"), ECVF_Cheat);


// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	TraceDistance = 500.f;
	CollisionChannel = ECC_WorldDynamic;
	TraceRadius = 30.f;
}

void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	
}



void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* Pawn = Cast<APawn>(GetOwner());
	if (Pawn->IsLocallyControlled())
	{
		LookForInteract();
	}
}

// when you look at an interactable object, a widget should appear on the screen saying "F to interact"
// line tracing every frame (seems kinda slow?)
// if you are looking at an actor that is interactable, add the widget
// if you look away, remove the widget
void USInteractionComponent::LookForInteract()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);
	
	FVector StartLocation;
	FRotator Rotation;
	GetOwner()->GetActorEyesViewPoint(StartLocation, Rotation);
	FVector EndLocation = StartLocation + (Rotation.Vector() * TraceDistance);
	
	TArray<FHitResult> HitResults;
	GetWorld()->SweepMultiByObjectType(HitResults, StartLocation, EndLocation, FQuat::Identity, ObjectQueryParams, FCollisionShape::MakeSphere(TraceRadius));

	FocusedActor = nullptr;

	for (FHitResult HitResult : HitResults)
	{
		if (AActor* HitActor = HitResult.GetActor())
		{
			if (HitActor->Implements<USGameplayInterface>())
			{
				FocusedActor = HitActor;
				break;
			}
		}
	}
	if (FocusedActor)
	{
		if (!DefaultWidgetInstance && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<USWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachedActor = FocusedActor;
			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}
	}
	else
	{
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}
}

void USInteractionComponent::ServerInteract_Implementation(AActor* InFocus)
{
	if (!InFocus)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, "No focus actor to interact");
		return;
	}
	ISGameplayInterface::Execute_Interact(InFocus, Cast<APawn>(GetOwner()));
}

void USInteractionComponent::PrimaryInteract()
{
	ServerInteract(FocusedActor);
}

