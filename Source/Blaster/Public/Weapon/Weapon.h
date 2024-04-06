// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponTypes.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial UMETA(DisplayName = "Initial State"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	EWS_Dropped UMETA(DisplayName = "Dropped"),

	EWS_MAX UMETA(DisplayName = "DefaultMax")
};

/**
 *
 */
UCLASS()
class BLASTER_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	AWeapon();

	void ShowPickupWidget(bool bShowWidget);
	virtual void Fire(const FVector& HitTarget);
	void Dropped();
	void SetHUDAmmo();
	void AddAmmo(int32 AmmoToAdd);

	/**
	 * Textures for crosshairs
	 */

	UPROPERTY(EditAnywhere, Category = "Crosshairs")
	class UTexture2D* CrosshairsCenter;

	UPROPERTY(EditAnywhere, Category = "Crosshairs")
	class UTexture2D* CrosshairsLeft;

	UPROPERTY(EditAnywhere, Category = "Crosshairs")
	class UTexture2D* CrosshairsRight;

	UPROPERTY(EditAnywhere, Category = "Crosshairs")
	class UTexture2D* CrosshairsTop;

	UPROPERTY(EditAnywhere, Category = "Crosshairs")
	class UTexture2D* CrosshairsBottom;

	/*
	 * Zoomed FOV While Aiming
	 */

	UPROPERTY(EditAnywhere)
	float ZoomedFOV = 30.0f;

	UPROPERTY(EditAnywhere)
	float ZoomInterpSpeed = 20.0f;

	/*
	 * Automatic Fire
	 */

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireDelay = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	bool bAutomatic = true;

	UPROPERTY(EditAnywhere)
	class USoundCue* EquipSound;

	/*
	 * CustomDepth
	 */

	void EnableCustomDepth(bool bEnable);

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnRep_Owner() override;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	class USphereComponent* AreaSphere;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	class UWidgetComponent* PickupWidget;

	UPROPERTY(EditAnywhere, Category = "WeaponProperties")
	class UAnimationAsset* FireAnimation;

	TObjectPtr<class ABlasterCharacter> BlasterOwnerCharacter;
	TObjectPtr<class ABlasterPlayerController> BlasterOwnerController;

	UPROPERTY(ReplicatedUsing = OnRep_WeaponState, VisibleAnywhere, Category = "Weapon Properties")
	EWeaponState WeaponState;

	UFUNCTION()
	void OnRep_WeaponState();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACasing> CasingClass;

	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_Ammo)
	int32 Ammo = 30;

	UFUNCTION()
	void OnRep_Ammo();

	void SpendRound();

	UPROPERTY(EditAnywhere)
	int32 MagCapacity = 30;

	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType;

public:
	void SetWeaponState(EWeaponState State);
	USphereComponent* GetAreaSphere() const {return AreaSphere;}
	USkeletalMeshComponent* GetWeaponMesh() const {return WeaponMesh;}
	float GetZoomedFOV() const {return ZoomedFOV;};
	float GetZoomInterpSpeed() const {return ZoomInterpSpeed;}
	bool IsEmpty();
	bool IsFull();
	EWeaponType GetWeaponType() const {return WeaponType;}
	int32 GetAmmo() const {return Ammo;}
	int32 GetMagCapacity() const {return MagCapacity;}

};