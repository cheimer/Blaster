// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/CaptureFlagGameMode.h"

#include "CaptureFlag/FlagZone.h"
#include "GameState/BlasterGameState.h"
#include "Weapon/Flag.h"

void ACaptureFlagGameMode::PlayerEliminated(ABlasterCharacter* EliCharacter,
                                            ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController)
{
	//Super::PlayerEliminated(EliCharacter, VictimController, AttackerController);
	ABlasterGameMode::PlayerEliminated(EliCharacter, VictimController, AttackerController);

}

void ACaptureFlagGameMode::FlagCaptured(AFlag* Flag, AFlagZone* Zone)
{
	bool bValidCapture = Flag->GetTeam() != Zone->Team;
	ABlasterGameState* BlasterGameState = Cast<ABlasterGameState>(GameState);
	if(BlasterGameState && bValidCapture)
	{
		if(Zone->Team == ETeam::ET_BlueTeam)
		{
			BlasterGameState->BlueTeamScores();
		}
		if(Zone->Team == ETeam::ET_RedTeam)
		{
			BlasterGameState->RedTeamScores();
		}
	}
}
