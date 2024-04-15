// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/HitScanWeapon.h"
#include "ShotgunWeapon.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API AShotgunWeapon : public AHitScanWeapon
{
	GENERATED_BODY()

public:
	virtual void FireShotgun(const TArray<FVector_NetQuantize>& HitTargets);

	void ShotgunTraceEndWithScatter(const FVector& HitTarget, TArray<FVector_NetQuantize>& OutHitTargets);

private:
	UPROPERTY(EditAnywhere, Category = "Scatter")
	uint32 NumOfPellets = 10;
	
};
