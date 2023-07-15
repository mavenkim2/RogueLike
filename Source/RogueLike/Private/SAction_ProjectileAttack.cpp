// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_ProjectileAttack.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

USAction_ProjectileAttack::USAction_ProjectileAttack()
{
	HandSocketName = "Muzzle_01";
	AttackAnimationDelay = 0.2f;
}

void USAction_ProjectileAttack::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);
	ACharacter* Character = Cast<ACharacter>(InstigatorActor);
	if (Character)
	{
		Character->PlayAnimMontage(AttackAnimation);
		UGameplayStatics::SpawnEmitterAttached(CastingEffect, Character->GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);

		FTimerHandle TimerHandle_AttackDelay;
		FTimerDelegate ProjectileDelegate; 
		ProjectileDelegate.BindUFunction(this, "AttackDelay_Elapsed", InstigatorActor);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, ProjectileDelegate, AttackAnimationDelay, false);
	}
}

void USAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* Instigator)
{
	if (ensureAlways(ProjectileClass))
	{
		FVector SpawnLocation = Instigator->GetMesh()->GetSocketLocation(HandSocketName);
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParameters.Instigator = Instigator;

		FCollisionObjectQueryParams QueryParams;
		QueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		QueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		QueryParams.AddObjectTypesToQuery(ECC_Pawn);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(Instigator);
	
		FHitResult HitResult;
		FVector StartLocation = Instigator->GetPawnViewLocation();
		FVector EndLocation = StartLocation + (Instigator->GetControlRotation().Vector() * 5000);

		// if (GetWorld()->SweepSingleByObjectType(HitResult, StartLocation, EndLocation, FQuat::Identity, QueryParams, FCollisionShape::MakeSphere(20.f), Params))
		if (GetWorld()->LineTraceSingleByObjectType(HitResult, StartLocation, EndLocation, QueryParams, Params))
		{
			EndLocation = HitResult.ImpactPoint;
		}
	
		FRotator ProjectileRotation = FRotationMatrix::MakeFromX(EndLocation - SpawnLocation).Rotator();
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLocation, ProjectileRotation, SpawnParameters);
	}

	StopAction(Instigator);
}




