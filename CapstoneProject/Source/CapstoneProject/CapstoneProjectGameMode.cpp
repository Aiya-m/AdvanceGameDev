// Copyright Epic Games, Inc. All Rights Reserved.

#include "CapstoneProjectGameMode.h"
#include "C_CapstoneProjectGameState.h"

ACapstoneProjectGameMode::ACapstoneProjectGameMode()
{
	GameStateClass = AC_CapstoneProjectGameState::StaticClass();
}

#include "TimerManager.h"

void ACapstoneProjectGameMode::OnPlayerDied(AController* VictimController)
{
	if (!VictimController) return;

	if (APawn* DeadPawn = VictimController->GetPawn())
	{
		DeadPawn->Destroy();
	}

	UE_LOG(LogTemp, Warning, TEXT("Player %s will respawn in %f seconds..."), *GetNameSafe(VictimController), RespawnDelay);
}

void ACapstoneProjectGameMode::RespawnPlayer(AController* ControllerToRespawn)
{
	if (ControllerToRespawn)
	{
		// ฟังก์ชัน Built-in ของ GameMode ในการ Respawn ตัวละครที่ PlayerStart
		RestartPlayer(ControllerToRespawn);
		UE_LOG(LogTemp, Warning, TEXT("Player %s Has Respawned!"), *GetNameSafe(ControllerToRespawn));
	}
}

void ACapstoneProjectGameMode::AddTeamScore(int32 Scores)
{
	if (!HasAuthority() || Scores <= 0) {return;}
	
	AC_CapstoneProjectGameState* GS = GetGameState<AC_CapstoneProjectGameState>();
	
	if (GS)
	{
		GS->AddTeamScore(Scores);
	}
}

void ACapstoneProjectGameMode::TravelToArena()
{
	GetWorld()->ServerTravel(TEXT("/Game/Maps/Battleground?listen"));
}

void ACapstoneProjectGameMode::TravelToLobby()
{
	GetWorld()->ServerTravel(TEXT("/Game/Maps/Lobby?listen"));
}
