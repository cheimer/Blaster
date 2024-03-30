// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "BlasterTypes/TurningInPlace.h"
#include "Interfaces/InteractWithCrosshairsInterface.h"
#include "BlasterCharacter.generated.h"

UCLASS()
class BLASTER_API ABlasterCharacter : public ACharacter, public IInteractWithCrosshairsInterface
{
	GENERATED_BODY()

public:
	ABlasterCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;

	void PlayFireMontage(bool bAiming);
	void PlayHitReactMontage();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastHit();

	virtual void OnRep_ReplicatedMovement() override;

protected:
	virtual void BeginPlay() override;

	void MoveForwardRight(const FInputActionValue& Value);
	void LookUpRight(const FInputActionValue& Value);
	void EquipButtonPressed(const FInputActionValue& Value);
	void CrouchButtonPressed(const FInputActionValue& Value);
	void AimButtonPressedAndReleased(const FInputActionValue& Value);
	void CalculateAO_Pitch();
	void AimOffSet(float DeltaTime);
	void SimProxiesTurn();
	virtual void Jump() override;
	void FireButtonPressedAndReleased(const FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput")
	TObjectPtr<class UInputMappingContext> PlayerInputMapping;
	UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput")
	TObjectPtr<class UInputAction> MoveAction;
	UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput")
	TObjectPtr<class UInputAction> LookAction;
	UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput")
	TObjectPtr<class UInputAction> JumpAction;
	UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput")
	TObjectPtr<class UInputAction> EquipAction;
	UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput")
	TObjectPtr<class UInputAction> CrouchAction;
	UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput")
	TObjectPtr<class UInputAction> AimAction;
	UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput")
	TObjectPtr<class UInputAction> FireAction;

private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<class UCameraComponent> FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* OverheadWidget;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCombatComponent> Combat;

	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	class AWeapon* OverlappingWeapon;

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
	class UAnimMontage* HitReactMontage;

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

};
