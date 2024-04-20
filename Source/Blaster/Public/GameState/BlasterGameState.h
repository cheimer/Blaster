// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BlasterGameState.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API ABlasterGameState : public AGameState
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&) const override;

	void UpdateTopScore(class ABlasterPlayerState* ScoringPlayer);

	UPROPERTY(Replicated)
	TArray<ABlasterPlayerState*> TopScoringPlayers;

	/*
	 * Teams
	 */
	TArray<ABlasterPlayerState*> RedTeam;
	TArray<ABlasterPlayerState*> BlueTeam;

	UPROPERTY(ReplicatedUsing = "OnRep_RedTeamScore")
	float RedTeamScore = 0.0f;
	UPROPERTY(ReplicatedUsing = "OnRep_BlueTeamScore")
	float BlueTeamScore = 0.0f;

	UFUNCTION()
	void OnRep_RedTeamScore();
	UFUNCTION()
	void OnRep_BlueTeamScore();

private:
	float TopScore = 0.0f;

};
