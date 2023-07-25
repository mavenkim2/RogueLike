// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

#include "SSaveGame.h"
#include "Net/UnrealNetwork.h"

ASPlayerState::ASPlayerState()
{
	Credits = 0;
}

int32 ASPlayerState::GetCredits() const
{
	return Credits;
}



bool ASPlayerState::ApplyCreditsChange(int32 Delta)
{
	if (Delta < 0.f && Credits + Delta < 0)
	{
		return false;
	}
	Credits += Delta;
	OnCreditsChanged.Broadcast(Credits, Delta);
	return true;
}

void ASPlayerState::LoadPlayerState_Implementation(USSaveGame* SaveGame)
{
	if (SaveGame)
	{
		ApplyCreditsChange(SaveGame->Credits);
	}
}

void ASPlayerState::SavePlayerState_Implementation(USSaveGame* SaveGame)
{
	if (SaveGame)
	{
		SaveGame->Credits = Credits;
	}
}

void ASPlayerState::OnRep_Credits(int32 OldCredits)
{
	OnCreditsChanged.Broadcast(Credits, Credits-OldCredits);
}

void ASPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASPlayerState, Credits);
}