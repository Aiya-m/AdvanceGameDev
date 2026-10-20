// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "C_CapstoneProjectGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTeamScoreChanged, int32, NewScore);

UCLASS()
class CAPSTONEPROJECT_API AC_CapstoneProjectGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(ReplicatedUsing= OnRep_TeamScore, BlueprintReadOnly, Category = "Score")
	int32 TeamScore = 0;
	
	UPROPERTY(BlueprintAssignable, Category="Score")
	FOnTeamScoreChanged OnTeamScoreChanged;
	
	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetTeamScore() const { return TeamScore; }
	
	void AddTeamScore(int32 Scores);
	
protected:
	UFUNCTION()
	void OnRep_TeamScore();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
