// Copyright Epic Games, Inc. All Rights Reserved.

#include "CapstoneProjectGameMode.h"

ACapstoneProjectGameMode::ACapstoneProjectGameMode()
{
	// stub
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