// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuffComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLASTER_API UBuffComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	friend class ABlasterCharacter;
	UBuffComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Heal(float InHealAmount, float InHealingTime);
	void ShieldRepairing(float InShieldAmount, float InShieldRepairTime);
	void SetInitialSpeed(float StandSpeed, float CrouchSpeed);
	void SpeedBuff(float BuffStandSpeed, float BuffCrouchSpeed, float BuffTime);
	void SetInitialJumpVelocity(float JumpVelocity);
	void JumpBuff(float BuffJumpVelocity, float BuffTime);

protected:
	virtual void BeginPlay() override;

	void HealRampUp(float DeltaTime);
	void ShieldRampUp(float DeltaTime);

private:
	TObjectPtr<class ABlasterCharacter> Character;

	/*
	 * Heal
	 */
	bool bHealing = false;
	float HealingRate = 0.0f;
	float HealAmount = 0.0f;

	/*
	 * Shield
	 */
	bool bShieldRepairing = false;
	float ShieldRepairRate = 0.0f;
	float ShieldAmount = 0.0f;

	/*
	 * Speed
	 */
	FTimerHandle SpeedBuffTimer;
	void ResetSpeeds();
	float InitialStandSpeed;
	float InitialCrouchSpeed;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSpeedBuff(float StandSpeed, float CrouchSpeed);

	/*
	 * Jump
	 */
	FTimerHandle JumpBuffTimer;
	void ResetJump();
	float InitialJumpVelocity;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastJumpBuff(float JumpVelocity);

};
