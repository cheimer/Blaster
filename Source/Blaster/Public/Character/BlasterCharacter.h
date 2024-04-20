// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "BlasterTypes/CombatState.h"
#include "BlasterTypes/Team.h"
#include "BlasterTypes/TurningInPlace.h"
#include "Components/TimelineComponent.h"
#include "Interfaces/InteractWithCrosshairsInterface.h"
#include "BlasterCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLeftGame);

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
	void PlayThrowGrenadeMontage();
	void PlaySwapMontage();

	virtual void OnRep_ReplicatedMovement() override;

	void Eliminated(bool bPlayerLeftGame);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastEliminated(bool bPlayerLeftGame);

	UPROPERTY(Replicated)
	bool bDisableGameplay = false;

	UFUNCTION(BlueprintImplementableEvent)
	void ShowSniperScopeWidget(bool bShowScope);

	void UpdateHUDHealth();
	void UpdateHUDShield();
	void UpdateHUDAmmo();

	void SpawnDefaultWeapon();

	UPROPERTY()
	TMap<FName, class UBoxComponent*> HitCollisionBoxes;

	bool bFinishedSwapping = false;

	UFUNCTION(Server, Reliable)
	void ServerLeaveGame();

	FOnLeftGame OnLeftGame;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastGainedTheLead();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastLostTheLead();

	void SetTeamColor(ETeam Team);

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
	void ThrowGrenadeButtonPressed(const FInputActionValue& Value);

	void CalculateAO_Pitch();
	void AimOffSet(float DeltaTime);
	void SimProxiesTurn();

	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	// Poll for any relevant classes and initialize our HUD
	void PollInit();
	void RotateInPlace(float DeltaTime);

	void DropOrDestroyWeapon(AWeapon* Weapon);

	/*
	 * Hit boxes for server rewind
	*/
	UPROPERTY(EditAnywhere)
	UBoxComponent* head;
	UPROPERTY(EditAnywhere)
	UBoxComponent* pelvis;
	UPROPERTY(EditAnywhere)
	UBoxComponent* spine_02;
	UPROPERTY(EditAnywhere)
	UBoxComponent* spine_03;
	UPROPERTY(EditAnywhere)
	UBoxComponent* upperarm_l;
	UPROPERTY(EditAnywhere)
	UBoxComponent* upperarm_r;
	UPROPERTY(EditAnywhere)
	UBoxComponent* lowerarm_l;
	UPROPERTY(EditAnywhere)
	UBoxComponent* lowerarm_r;
	UPROPERTY(EditAnywhere)
	UBoxComponent* hand_l;
	UPROPERTY(EditAnywhere)
	UBoxComponent* hand_r;
	UPROPERTY(EditAnywhere)
	UBoxComponent* backpack;
	UPROPERTY(EditAnywhere)
	UBoxComponent* blanket;
	UPROPERTY(EditAnywhere)
	UBoxComponent* thigh_l;
	UPROPERTY(EditAnywhere)
	UBoxComponent* thigh_r;
	UPROPERTY(EditAnywhere)
	UBoxComponent* calf_l;
	UPROPERTY(EditAnywhere)
	UBoxComponent* calf_r;
	UPROPERTY(EditAnywhere)
	UBoxComponent* foot_l;
	UPROPERTY(EditAnywhere)
	UBoxComponent* foot_r;

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
	UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput")
	UInputAction* ThrowGrenadeAction;

private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* OverheadWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCombatComponent* Combat;

	UPROPERTY(VisibleAnywhere)
	class UBuffComponent* Buff;

	UPROPERTY(VisibleAnywhere)
	class ULagCompensationComponent* LagCompensation;

	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	class AWeapon* OverlappingWeapon;

	TObjectPtr<class ABlasterPlayerState> BlasterPlayerState;
	TObjectPtr<class ABlasterGameMode> BlasterGameMode;

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
	UAnimMontage* FireWeaponMontage;
	UPROPERTY(EditAnywhere, Category = "Combat")
	UAnimMontage* ReloadMontage;
	UPROPERTY(EditAnywhere, Category = "Combat")
	UAnimMontage* HitReactMontage;
	UPROPERTY(EditAnywhere, Category = "Combat")
	UAnimMontage* EliminatedMontage;
	UPROPERTY(EditAnywhere, Category = "Combat")
	UAnimMontage* ThrowGrenadeMontage;
	UPROPERTY(EditAnywhere, Category = "Combat")
	UAnimMontage* SwapMontage;

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

	UPROPERTY(ReplicatedUsing = OnRep_Health, EditAnywhere, Category = "Player Stats")
	float Health = 100.0f;

	UFUNCTION()
	void OnRep_Health(float LastHealth);

	/*
	 * Player Shield
	 */

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float MaxShield = 100.0f;

	UPROPERTY(ReplicatedUsing = OnRep_Shield, EditAnywhere, Category = "Player Stats")
	float Shield = 100.0f;

	UFUNCTION()
	void OnRep_Shield(float LastShield);

	TObjectPtr<class ABlasterPlayerController> BlasterPlayerController;

	bool bEliminated = false;

	FTimerHandle EliminatedTimer;

	UPROPERTY(EditDefaultsOnly)
	float EliminatedDelay = 3.0f;

	void EliminatedTimerFinished();

	bool bLeftGame = false;

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
	UPROPERTY(VisibleAnywhere, Category = "Eliminated")
	UMaterialInstance* DissolveMaterialInstance;

	/*
	 * Team Colors
	 */

	UPROPERTY(EditAnywhere, Category = "Eliminated")
	UMaterialInstance* RedDissolveMatInst;

	UPROPERTY(EditAnywhere, Category = "Eliminated")
	UMaterialInstance* RedMaterial;

	UPROPERTY(EditAnywhere, Category = "Eliminated")
	UMaterialInstance* BlueDissolveMatInst;

	UPROPERTY(EditAnywhere, Category = "Eliminated")
	UMaterialInstance* BlueMaterial;

	UPROPERTY(EditAnywhere, Category = "Eliminated")
	UMaterialInstance* OriginMaterial;

	/*
	 * Eli effects
	 */

	UPROPERTY(EditAnywhere)
	UParticleSystem* EliBotEffect;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* EliBotComponent;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* CrownSystem;

	TObjectPtr<class UNiagaraComponent> CrownComponent;

	/*
	 * Grenade
	 */

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* AttachedGrenade;

	/*
	 * Default Weapon
	 */

	 UPROPERTY(EditAnywhere)
	 TSubclassOf<class AWeapon> DefaultWeaponClass;


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
	void SetHealth(float Amount) {Health = Amount;}
	float GetMaxHealth() const {return MaxHealth;}
	float GetShield() const {return Shield;}
	void SetShield(float Amount) {Shield = Amount;}
	float GetMaxShield() const {return MaxShield;}
	ECombatState GetCombatState() const;
	UCombatComponent* GetCombat() const {return Combat;}
	bool GetDisableGameplay() const {return bDisableGameplay;}
	UAnimMontage* GetReloadMontage() const {return ReloadMontage;}
	UStaticMeshComponent* GetAttachedGrenade() const {return AttachedGrenade;}
	UBuffComponent* GetBuff() const {return Buff;}
	bool IsLocallyReloading();
	ULagCompensationComponent* GetLagCompensation() const {return LagCompensation;}

};
