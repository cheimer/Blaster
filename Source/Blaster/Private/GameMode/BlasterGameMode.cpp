// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/BlasterGameMode.h"

#include "Character/BlasterCharacter.h"
#include "GameFramework/PlayerStart.h"
#include "GameState/BlasterGameState.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerController/BlasterPlayerController.h"
#include "PlayerState/BlasterPlayerState.h"

namespace MatchState
{
	const FName Cooldown = FName("Cooldown");
}

ABlasterGameMode::ABlasterGameMode()
{
	bDelayedStart = true;
}

void ABlasterGameMode::BeginPlay()
{
	Super::BeginPlay();

	LevelStartingTime = GetWorld()->GetTimeSeconds();
}

void ABlasterGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(MatchState == MatchState::WaitingToStart)
	{
		CountDownTime = WarmupTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if(CountDownTime <= 0.0f)
		{
			StartMatch();
		}
	}
	else if(MatchState == MatchState::InProgress)
	{
		CountDownTime = WarmupTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if(CountDownTime <= 0.0f)
		{
			SetMatchState(MatchState::Cooldown);
		}
	}
	else if(MatchState == MatchState::Cooldown)
	{
		CountDownTime = CooldownTime + WarmupTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if(CountDownTime <= 0.0f)
		{
			RestartGame();
		}
	}
}

void ABlasterGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();

	for(FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ABlasterPlayerController* BlasterPlayerController = Cast<ABlasterPlayerController>(*It);
		if(BlasterPlayerController)
		{
			BlasterPlayerController->OnMatchStateSet(MatchState);
		}
	}
}

void ABlasterGameMode::PlayerEliminated(ABlasterCharacter* EliCharacter,
                                        ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController)
{
	ABlasterPlayerState* AttackerPlayerState = AttackerController ? Cast<ABlasterPlayerState>(AttackerController->PlayerState) : nullptr;
	ABlasterPlayerState* VictimPlayerState = VictimController ? Cast<ABlasterPlayerState>(VictimController->PlayerState) : nullptr;
	if(!AttackerPlayerState || !VictimPlayerState) return;

	ABlasterGameState* BlasterGameState = GetGameState<ABlasterGameState>();

	if(AttackerPlayerState && AttackerPlayerState != VictimPlayerState && BlasterGameState)
	{
		TArray<ABlasterPlayerState*> PlayerCurrentlyInTheLead;
		for(auto LeadPlayer : BlasterGameState->TopScoringPlayers)
		{
			PlayerCurrentlyInTheLead.Emplace(LeadPlayer);
		}

		AttackerPlayerState->AddToScore(1.0f);
		BlasterGameState->UpdateTopScore(AttackerPlayerState);
		if(BlasterGameState->TopScoringPlayers.Contains(AttackerPlayerState))
		{
			ABlasterCharacter* Leader = Cast<ABlasterCharacter>(AttackerPlayerState->GetPawn());
			if(Leader)
			{
				Leader->MulticastGainedTheLead();
			}
		}

		for(int32 i = 0; i < PlayerCurrentlyInTheLead.Num(); i++)
		{
			if(!BlasterGameState->TopScoringPlayers.Contains(PlayerCurrentlyInTheLead[i]))
			{
				ABlasterCharacter* Loser = Cast<ABlasterCharacter>(PlayerCurrentlyInTheLead[i]->GetPawn());
				if(Loser)
				{
					Loser->MulticastLostTheLead();
				}
			}
		}
	}
	if(VictimPlayerState)
	{
		AttackerPlayerState->AddToDefeats(1);
	}

	if(EliCharacter)
	{
		EliCharacter->Eliminated(false);
	}

	for(FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ABlasterPlayerController* BlasterPlayerController = Cast<ABlasterPlayerController>(*It);
		if(BlasterPlayerController && AttackerPlayerState && VictimPlayerState)
		{
			BlasterPlayerController->BroadcastEli(AttackerPlayerState, VictimPlayerState);
		}
	}
}

void ABlasterGameMode::RequestRespawn(ACharacter* EliCharacter, AController* EliController)
{
	if(EliCharacter)
	{
		EliCharacter->Reset();
		EliCharacter->Destroy();
	}
	if(EliController)
	{
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
		int32 Selection = FMath::RandRange(0, PlayerStarts.Num() - 1);
		RestartPlayerAtPlayerStart(EliController, PlayerStarts[Selection]);
	}
}

void ABlasterGameMode::PlayerLeftGame(ABlasterPlayerState* PlayerLeaving)
{
	if(!PlayerLeaving) return;

	ABlasterGameState* BlasterGameState = GetGameState<ABlasterGameState>();
	if(BlasterGameState && BlasterGameState->TopScoringPlayers.Contains(PlayerLeaving))
	{
		BlasterGameState->TopScoringPlayers.Remove(PlayerLeaving);
	}
	ABlasterCharacter* CharacterLeaving = Cast<ABlasterCharacter>(PlayerLeaving->GetPawn());
	if(CharacterLeaving)
	{
		CharacterLeaving->Eliminated(true);
	}
}
