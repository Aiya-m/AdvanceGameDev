// Fill out your copyright notice in the Description page of Project Settings.


#include "C_CapstoneProjectGameState.h"
#include "Net/UnrealNetwork.h"

void AC_CapstoneProjectGameState::AddTeamScore(int32 Scores)
{
	if (!HasAuthority() || Scores <= 0)
	{
		return;
	}
	
	TeamScore += Scores;
	
	OnTeamScoreChanged.Broadcast(TeamScore);
}

void AC_CapstoneProjectGameState::OnRep_TeamScore()
{
	OnTeamScoreChanged.Broadcast(TeamScore);
}

void AC_CapstoneProjectGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AC_CapstoneProjectGameState, TeamScore);
}
