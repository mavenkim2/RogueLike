// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionEffect_Thorns.h"

#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SGameplayFunctionLibrary.h"

USActionEffect_Thorns::USActionEffect_Thorns()
{
    ReflectPercent = .1;
    Duration = 0.0f;
    Period = 0.0f;
}

void USActionEffect_Thorns::StartAction_Implementation(AActor* InstigatorActor)
{
    Super::StartAction_Implementation(InstigatorActor);

    // thorns
    // when owner takes damage, reflect damage back to instigator
    // bind to on health changed of owner
    // stretch: make sure that thorns damage can't be reflected by thorns

    USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(Cast<USActionComponent>(GetOuter())->GetOwner());
    if (AttributeComponent)
    {
        AttributeComponent->OnHealthChanged.AddDynamic(this, &USActionEffect_Thorns::OnHealthChanged);
    }
}

void USActionEffect_Thorns::StopAction_Implementation(AActor* InstigatorActor)
{
    Super::StopAction_Implementation(InstigatorActor);
    USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(Cast<USActionComponent>(GetOuter())->GetOwner());
    if (AttributeComponent)
    {
        AttributeComponent->OnHealthChanged.RemoveDynamic(this, &USActionEffect_Thorns::OnHealthChanged);
    }
}

void USActionEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
    AActor* OwningActor = OwningComp->GetOwner();

    if (Delta < 0.f && OwningActor != InstigatorActor)
    {
        int32 ReflectedDamage = FMath::RoundToInt(Delta * ReflectPercent);
        if (ReflectedDamage == 0)
        {
            return;
        }
        ReflectedDamage = FMath::Abs(ReflectedDamage);
        USGameplayFunctionLibrary::ApplyDamage(OwningComp->GetOwner(), InstigatorActor, ReflectedDamage);
    }
}