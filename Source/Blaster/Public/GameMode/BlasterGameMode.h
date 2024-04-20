// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BlasterGameMode.generated.h"

namespace MatchState
{
	extern BLASTER_API const FName Cooldown; // Display Winner and begin
}

/**
 *
 */
UCLASS()
class BLASTER_API ABlasterGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ABlasterGameMode();
	virtual void Tick(float DeltaSeconds) override;

	virtual void PlayerEliminated(class ABlasterCharacter* EliCharacter,
		class ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController);

	virtual void RequestRespawn(class ACharacter* EliCharacter, AController* EliController);

	void PlayerLeftGame(class ABlasterPlayerState* PlayerLeaving);

	virtual float CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage);

	UPROPERTY(EditDefaultsOnly)
	float WarmupTime = 5.0f;

	UPROPERTY(EditDefaultsOnly)
	float MatchTime = 120.0f;

	UPROPERTY(EditDefaultsOnly)
	float CooldownTime = 10.0f;

	float LevelStartingTime = 0.0f;

protected:
	virtual void BeginPlay() override;

	virtual void OnMatchStateSet() override;

private:
	float CountDownTime = 0.0f;

public:
	float GetCountDownTime() const {return CountDownTime;}

};
