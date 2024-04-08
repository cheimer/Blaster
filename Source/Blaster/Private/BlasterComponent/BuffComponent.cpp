// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterComponent/BuffComponent.h"

#include "Character/BlasterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UBuffComponent::UBuffComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UBuffComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UBuffComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	HealRampUp(DeltaTime);
	ShieldRampUp(DeltaTime);
}

void UBuffComponent::Heal(float InHealAmount, float InHealingTime)
{
	bHealing = true;
	HealingRate = InHealAmount / InHealingTime;
	HealAmount += InHealAmount;

}

void UBuffComponent::HealRampUp(float DeltaTime)
{
	if(!bHealing || !Character || Character->IsEliminated()) return;

	const float HealThisFrame = HealingRate * DeltaTime;
	Character->SetHealth(FMath::Clamp(Character->GetHealth() + HealThisFrame, 0.0f, Character->GetMaxHealth()));
	Character->UpdateHUDHealth();

	HealAmount -= HealThisFrame;
	if(HealAmount <= 0 || Character->GetHealth() >= Character->GetMaxHealth())
	{
		bHealing = false;
		HealAmount = 0.0f;
	}
}

void UBuffComponent::ShieldRepairing(float InShieldAmount, float InShieldRepairTime)
{
	bShieldRepairing = true;
	ShieldRepairRate = InShieldAmount / InShieldRepairTime;
	ShieldAmount += InShieldAmount;

}

void UBuffComponent::ShieldRampUp(float DeltaTime)
{
	if(!bShieldRepairing || !Character || Character->IsEliminated()) return;

	const float ShieldRepairThisFrame = ShieldRepairRate * DeltaTime;
	Character->SetShield(FMath::Clamp(Character->GetShield() + ShieldRepairThisFrame, 0.0f, Character->GetMaxShield()));
	Character->UpdateHUDShield();

	ShieldAmount -= ShieldRepairThisFrame;
	if(ShieldAmount <= 0 || Character->GetShield() >= Character->GetMaxShield())
	{
		bShieldRepairing = false;
		ShieldAmount = 0.0f;
	}
}

void UBuffComponent::SetInitialSpeed(float StandSpeed, float CrouchSpeed)
{
	InitialStandSpeed = StandSpeed;
	InitialCrouchSpeed = CrouchSpeed;
}

void UBuffComponent::SpeedBuff(float BuffStandSpeed, float BuffCrouchSpeed, float BuffTime)
{
	if(!Character || !Character->GetCharacterMovement()) return;
	Character->GetWorldTimerManager().SetTimer(SpeedBuffTimer, this, &ThisClass::ResetSpeeds, BuffTime);

	MulticastSpeedBuff(BuffStandSpeed, BuffCrouchSpeed);
}

void UBuffComponent::ResetSpeeds()
{
	if(!Character || !Character->GetCharacterMovement()) return;

	MulticastSpeedBuff(InitialStandSpeed, InitialCrouchSpeed);
}

void UBuffComponent::MulticastSpeedBuff_Implementation(float StandSpeed, float CrouchSpeed)
{
	if(!Character || !Character->GetCharacterMovement()) return;

	Character->GetCharacterMovement()->MaxWalkSpeed = StandSpeed;
	Character->GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed;
}

void UBuffComponent::SetInitialJumpVelocity(float JumpVelocity)
{
	InitialJumpVelocity = JumpVelocity;
}

void UBuffComponent::JumpBuff(float BuffJumpVelocity, float BuffTime)
{
	if(!Character || !Character->GetCharacterMovement()) return;
	Character->GetWorldTimerManager().SetTimer(JumpBuffTimer, this, &ThisClass::ResetJump, BuffTime);

	MulticastJumpBuff(BuffJumpVelocity);
}

void UBuffComponent::ResetJump()
{
	if(!Character || !Character->GetCharacterMovement()) return;

	MulticastJumpBuff(InitialJumpVelocity);
}

void UBuffComponent::MulticastJumpBuff_Implementation(float JumpVelocity)
{
	if(!Character || !Character->GetCharacterMovement()) return;

	Character->GetCharacterMovement()->JumpZVelocity = JumpVelocity;
}
