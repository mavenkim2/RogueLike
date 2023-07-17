// Fill out your copyright notice in the Description page of Project Settings.


#include "SCoin.h"

#include "SAttributeComponent.h"
#include "SPlayerState.h"

ASCoin::ASCoin()
{
	CreditsRewarded = 10;
	RespawnTime = 5.f;
}

void ASCoin::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);

	if (!ensure(InstigatorPawn))
	{
		return;
	}
	if (ASPlayerState* PlayerState = InstigatorPawn->GetPlayerState<ASPlayerState>())
	{
		if (PlayerState->ApplyCreditsChange(CreditsRewarded))
		{
			DeactivateAndCooldown();
		}
	}
}
