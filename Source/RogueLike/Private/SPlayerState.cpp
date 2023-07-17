// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

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
	// UE_LOG(LogTemp, Log, TEXT("%s has %i credits"), *GetNameSafe(GetOwner()), Credits);
	OnCreditsChanged.Broadcast(Credits, Delta);
	return true;
}
