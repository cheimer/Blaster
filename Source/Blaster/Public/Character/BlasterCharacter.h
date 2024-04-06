// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "BlasterTypes/CombatState.h"
#include "BlasterTypes/TurningInPlace.h"
#include "Components/TimelineComponent.h"
#include "Interfaces/InteractWithCrosshairsInterface.h"
#include "BlasterCharacter.generated.h"

UCLASS()
class BLASTER_API ABlasterCharacter : public ACharacter, public IInteractWithCrosshairsInterface
{
	GENERATED_BODY()

public:
	ABlasterCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;

	void PlayFireMontage(bool bAiming);
	void PlayReloadMontage();
	void PlayHitReactMontage();
	void PlayEliminatedMontage();

	virtual void OnRep_ReplicatedMovement() override;

	void Eliminated();

	UFUNCTION(NetMulticast, Reliable)
	void MultiEliminated();

	UPROPERTY(Replicated)
	bool bDisableGameplay = false;

	UFUNCTION(BlueprintImplementableEvent)
	void ShowSniperScopeWidget(bool bShowScope);

protected:
	virtual void BeginPlay() override;

	void MoveForwardRight(const FInputActionValue& Value);
	void LookUpRight(const FInputActionValue& Value);
	virtual void Jump() override;

	void EquipButtonPressed(const FInputActionValue& Value);
	void CrouchButtonPressed(const FInputActionValue& Value);
	void ReloadButtonPressed(const FInputActionValue& Value);
	void AimButtonPressedAndReleased(const FInputActionValue& Value);
	void FireButtonPressedAndReleased(const FInputActionValue& Value);

	void CalculateAO_Pitch();
	void AimOffSet(float DeltaTime);
	void SimProxiesTurn();

	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	void UpdateHUDHealth();
	// Poll for any relevant classes and initialize our HUD
	void PollInit();
	void RotateInPlace(float DeltaTime);

	UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput")
	class UInputMappingContext* PlayerInputMapping;
	UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput")
	class UInputAction* MoveAction;
	UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput")
	UInputAction* LookAction;
	UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput")
	UInputAction* JumpAction;
	UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput")
	UInputAction* EquipAction;
	UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput")
	UInputAction* CrouchAction;
	UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput")
	UInputAction* AimAction;
	UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput")
	UInputAction* FireAction;
	UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput")
	UInputAction* ReloadAction;

private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* OverheadWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCombatComponent* Combat;

	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	class AWeapon* OverlappingWeapon;

	TObjectPtr<class ABlasterPlayerState> BlasterPlayerState;

	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();

	float AO_Yaw;
	float InterpAO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;

	ETurningInPlace TurningInPlace;
	void TurnInPlace(float DeltaTime);

	UPROPERTY(EditAnywhere, Category = "Combat")
	class UAnimMontage* FireWeaponMontage;
	UPROPERTY(EditAnywhere, Category = "Combat")
	class UAnimMontage* ReloadMontage;
	UPROPERTY(EditAnywhere, Category = "Combat")
	class UAnimMontage* HitReactMontage;
	UPROPERTY(EditAnywhere, Category = "Combat")
	class UAnimMontage* EliminatedMontage;

	void HideCameraIfCharacterClose();

	UPROPERTY(EditAnywhere)
	float CameraThreshold = 200.0f;

	bool bRotateRootBone;
	float TurnThreshold = 0.5f;
	FRotator ProxyRotationLastFrame;
	FRotator ProxyRotation;
	float ProxyYaw;
	float TimeSinceLastMovementReplication;
	float CalculateSpeed();

	/*
	 * Player Health
	 */

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float MaxHealth = 100.0f;

	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = "Player Stats")
	float Health = 100.0f;

	UFUNCTION()
	void OnRep_Health();

	TObjectPtr<class ABlasterPlayerController> BlasterPlayerController;

	bool bEliminated = false;

	FTimerHandle EliminatedTimer;

	UPROPERTY(EditDefaultsOnly)
	float EliminatedDelay = 3.0f;

	void EliminatedTimerFinished();

	/*
	 * Dissolve Effect
	 */

	UPROPERTY(VisibleAnywhere)
	UTimelineComponent* DissolveTimeLine;

	FOnTimelineFloat DissolveTrack;

	void StartDissolve();
	UFUNCTION()
	void UpdateDissolveMaterial(float DissolveValue);

	UPROPERTY(EditAnywhere)
	UCurveFloat* DissolveCurve;

	// Dynamic Instance that We Can Change at Runtime
	UPROPERTY(VisibleAnywhere, Category = "Eliminated")
	UMaterialInstanceDynamic* DynamicDissolveMaterialInstance;

	// Material Instance Set on the Blueprint, used with the Dynamic Material Instance
	UPROPERTY(EditAnywhere, Category = "Eliminated")
	UMaterialInstance* DissolveMaterialInstance;

	/*
	 * Eli bot
	 */

	UPROPERTY(EditAnywhere)
	UParticleSystem* EliBotEffect;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* EliBotComponent;

public:
	void SetOverlappingWeapon(AWeapon* Weapon);
	bool IsWeaponEquipped();
	bool IsAiming();
	float GetAO_Yaw() const {return AO_Yaw;}
	float GetAO_Pitch() const {return AO_Pitch;}
	AWeapon* GetEquippedWeapon();
	ETurningInPlace GetTurningInPlace() const {return TurningInPlace;}
	FVector GetHitTarget() const;
	UCameraComponent* GetFollowCamera() const {return FollowCamera;}
	bool ShouldRotateRootBone() const {return bRotateRootBone;}
	bool IsEliminated() const {return bEliminated;}
	float GetHealth() const {return Health;}
	float GetMaxHealth() const {return MaxHealth;}
	ECombatState GetCombatState() const;
	UCombatComponent* GetCombat() const {return Combat;}
	bool GetDisableGameplay() const {return bDisableGameplay;}
	UAnimMontage* GetReloadMontage() {return ReloadMontage;}

};
