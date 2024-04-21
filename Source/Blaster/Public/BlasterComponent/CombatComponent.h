// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlasterTypes/CombatState.h"
#include "Components/ActorComponent.h"
#include "HUD/BlasterHUD.h"
#include "Weapon/WeaponTypes.h"
#include "CombatComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLASTER_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	friend class ABlasterCharacter;

	UCombatComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void EquipWeapon(class AWeapon* InEquipWeapon);
	void SwapWeapon();
	void Reload();
	UFUNCTION(BlueprintCallable)
	void FinishReloading();

	UFUNCTION(BlueprintCallable)
	void FinishSwapAttachWeapon();
	UFUNCTION(BlueprintCallable)
	void FinishSwap();

	void FireButtonPressed(bool bPressed);

	UFUNCTION(BlueprintCallable)
	void ShotgunShellReload();

	void JumpToShotgunEnd();

	UFUNCTION(BlueprintCallable)
	void ThrowGrenadeFinished();

	UFUNCTION(BlueprintCallable)
	void LaunchGrenade();

	UFUNCTION(Server, Reliable)
	void ServerLaunchGrenade(const FVector_NetQuantize& Target);

	void PickupAmmo(EWeaponType WeaponType, int32 AmmoAmount);

	bool bLocallyReloading = false;

protected:
	virtual void BeginPlay() override;

	void SetAiming(bool bAimingParam);

	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool bAimingParam);

	UFUNCTION()
	void OnRep_EquippedWeapon();

	UFUNCTION()
	void OnRep_SecondaryWeapon();

	void Fire();
	void FireProjectileWeapon();
	void FireHitScanWeapon();
	void FireShotgunWeapon();
	void LocalFire(const FVector_NetQuantize& TraceHitTarget);
	void ShotgunLocalFire(const TArray<FVector_NetQuantize>& TraceHitTarget);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFire(const FVector_NetQuantize& TraceHitTarget, float FireDelay);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastFire(const FVector_NetQuantize& TraceHitTarget);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerShotgunFire(const TArray<FVector_NetQuantize>& TraceHitTarget, float FireDelay);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastShotgunFire(const TArray<FVector_NetQuantize>& TraceHitTarget);

	void TraceUnderCrosshairs(FHitResult& TraceHitResult);

	void SetHUDCrosshairs(float DeltaTime);

	UFUNCTION(Server, Reliable)
	void ServerReload();

	void HandleReload();
	int32 AmountToReload();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> GrenadeClass;

	void ThrowGrenade();

	UFUNCTION(Server, Reliable)
	void ServerThrowGrenade();

	void DropEquippedWeapon();

	void AttachActorToRightHand(AActor* ActorToAttach);
	void AttachActorToLeftHand(AActor* ActorToAttach);
	void AttachFlagToLeftHand(AWeapon* Flag);
	void AttachActorToBackpack(AActor* ActorToAttach);

	void UpdateCarriedAmmo();

	void PlayEquipWeaponSound(AWeapon* InEquipWeapon);

	void ReloadEmptyWeapon();

	void ShowAttachedGrenade(bool bShowGrenade);

	void EquipPrimaryWeapon(AWeapon* InEquipWeapon);
	void EquipSecondaryWeapon(AWeapon* InEquipWeapon);

private:
	TObjectPtr<class ABlasterCharacter> Character;
	TObjectPtr<class ABlasterPlayerController> Controller;
	TObjectPtr<class ABlasterHUD> HUD;

	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	AWeapon* EquippedWeapon;

	UPROPERTY(ReplicatedUsing = OnRep_SecondaryWeapon)
	AWeapon* SecondaryWeapon;

	UPROPERTY(ReplicatedUsing = OnRep_Aiming)
	bool bAiming = false;

	bool bAimButtonPressed = false;

	UFUNCTION()
	void OnRep_Aiming();

	UPROPERTY(EditAnywhere)
	float BaseWalkSpeed;

	UPROPERTY(EditAnywhere)
	float AimWalkSpeed;

	bool bFireButtonPressed;

	/*
	 * HUD and Crosshairs
	 */

	FHUDPackage HUDPackage;

	float CrosshairVelocityFactor;
	float CrosshairInAirFactor;
	float CrosshairAimFactor;
	float CrosshairShootingFactor;

	FVector HitTarget;

	/*
	 * Aiming And FOV
	 */

	float DefaultFOV;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ZoomedFOV = 30.0f;

	float CurrentFOV;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ZoomInterpSpeed = 20.0f;

	void InterpFOV(float DeltaTime);

	/*
	 * Automatic Fire
	 */

	FTimerHandle FireTimer;

	bool bCanFire = true;

	void StartFireTimer();
	void FireTimerFinished();

	bool CanFire();

	UPROPERTY(ReplicatedUsing = OnRep_CarriedAmmo)
	int32 CarriedAmmo;

	UFUNCTION()
	void OnRep_CarriedAmmo();

	TMap<EWeaponType, int32> CarriedAmmoMap;

	UPROPERTY(EditAnywhere)
	int StartingARAmmo = 30;

	UPROPERTY(EditAnywhere)
	int StartingRocketAmmo = 4;

	UPROPERTY(EditAnywhere)
	int StartingPistolAmmo = 15;

	UPROPERTY(EditAnywhere)
	int StartingSMGAmmo = 20;

	UPROPERTY(EditAnywhere)
	int StartingShotgunAmmo = 5;

	UPROPERTY(EditAnywhere)
	int StartingSniperAmmo = 3;

	UPROPERTY(EditAnywhere)
	int StartingGrenadeLauncherAmmo = 5;

	void InitializeCarriedAmmo();

	UPROPERTY(ReplicatedUsing = OnRep_CombatState)
	ECombatState CombatState = ECombatState::ECS_Unoccupied;

	UFUNCTION()
	void OnRep_CombatState();

	void UpdateAmmoValues();
	void UpdateShotgunAmmoValues();

	UPROPERTY(ReplicatedUsing = OnRep_Grenades)
	int32 Grenades = 2;

	UFUNCTION()
	void OnRep_Grenades();

	UPROPERTY(EditAnywhere)
	int32 MaxGrenades = 4;

	void UpdateHUDGrenades();

	UPROPERTY(ReplicatedUsing = "OnRep_bHoldingTheFlag")
	bool bHoldingTheFlag = false;

	UFUNCTION()
	void OnRep_bHoldingTheFlag();

	TObjectPtr<AWeapon> TheFlag;

public:
	int32 GetGrenades() const {return Grenades;}
	bool ShouldSwapWeapon();

};
