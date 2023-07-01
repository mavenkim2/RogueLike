// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"

#include "SGameplayInterface.h"

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USInteractionComponent::PrimaryInteract()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	
	FVector StartLocation;
	FRotator Rotation;
	GetOwner()->GetActorEyesViewPoint(StartLocation, Rotation);
	FVector EndLocation = StartLocation + (Rotation.Vector() * 1000.0f);
	
	// FHitResult HitResult;
	// GetWorld()->LineTraceSingleByObjectType(HitResult, StartLocation, EndLocation, ObjectQueryParams);

	float Radius = 30.0f;
	FCollisionShape Sphere;
	Sphere.SetSphere(Radius);
	
	TArray<FHitResult> HitResults;
	bool bAreResults = GetWorld()->SweepMultiByObjectType(HitResults, StartLocation, EndLocation, FQuat::Identity, ObjectQueryParams, Sphere);
	FColor Color = bAreResults ? FColor::Green : FColor:: Red;

	for (FHitResult HitResult : HitResults)
	{
		if (AActor* HitActor = HitResult.GetActor())
		{
			if (HitActor->Implements<USGameplayInterface>())
			{
				ISGameplayInterface::Execute_Interact(HitActor, Cast<APawn>(GetOwner()));
				DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, Radius, 32.f, Color, false, 2.0f);
				break;
			}
		}
	}

	DrawDebugLine(GetWorld(), StartLocation, EndLocation, Color, false, 2.0f);
}

