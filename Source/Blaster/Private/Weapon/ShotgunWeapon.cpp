// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ShotgunWeapon.h"

#include "BlasterComponent/LagCompensationComponent.h"
#include "Character/BlasterCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "PlayerController/BlasterPlayerController.h"
#include "Sound/SoundCue.h"

void AShotgunWeapon::FireShotgun(const TArray<FVector_NetQuantize>& HitTargets)
{
	if(!GetWorld()) return;

	AWeapon::Fire(FVector());

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if(!OwnerPawn) return;

	AController* InstigatorController = OwnerPawn->GetController();

	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName("MuzzleFlash");
	if(MuzzleFlashSocket)
	{
		const FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		const FVector Start = SocketTransform.GetLocation();

		// Character - Numbers of hit by bullet
		TMap<ABlasterCharacter*, uint32> HitMap;

		for(FVector_NetQuantize HitTarget : HitTargets)
		{
			FHitResult FireHit;
			WeaponTraceHit(Start, HitTarget, FireHit);

			ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(FireHit.GetActor());
			if(BlasterCharacter)
			{
				if(HitMap.Contains(BlasterCharacter))
				{
					HitMap[BlasterCharacter]++;
				}
				else
				{
					HitMap.Emplace(BlasterCharacter, 1);
				}

				if(ImpactParticle)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle,
						FireHit.ImpactPoint, FireHit.ImpactNormal.Rotation());
				}
				if(HitSound)
				{
					UGameplayStatics::PlaySoundAtLocation(this, HitSound, FireHit.ImpactPoint,
						0.5f, FMath::FRandRange(-0.5f, 0.5f));
				}
			}

		}

		TArray<ABlasterCharacter*> HitCharacter;

		for(auto HitPair : HitMap)
		{
			if(HitPair.Key && InstigatorController)
			{
				bool bCauseAuthDamage = !bUseServerSideRewind || OwnerPawn->IsLocallyControlled();
				if(HasAuthority() && bCauseAuthDamage)
				{
					UGameplayStatics::ApplyDamage(HitPair.Key, Damage * HitPair.Value,
						InstigatorController, this, UDamageType::StaticClass());
				}
				HitCharacter.Emplace(HitPair.Key);
			}
		}
		if(!HasAuthority() && bUseServerSideRewind)
		{
			BlasterOwnerCharacter = !BlasterOwnerCharacter ? Cast<ABlasterCharacter>(OwnerPawn) : BlasterOwnerCharacter;
			BlasterOwnerController = !BlasterOwnerController ? Cast<ABlasterPlayerController>(InstigatorController) : BlasterOwnerController;
			bool IsValid =
				BlasterOwnerCharacter &&
				BlasterOwnerController &&
				BlasterOwnerCharacter->GetLagCompensation() &&
				BlasterOwnerCharacter->IsLocallyControlled();

			if(IsValid)
			{
				BlasterOwnerCharacter->GetLagCompensation()->ServerShotgunScoreRequest(HitCharacter, Start, HitTargets,
					BlasterOwnerController->GetServerTime() - BlasterOwnerController->SingleTripTime);
			}
		}
	}

}

void AShotgunWeapon::ShotgunTraceEndWithScatter(const FVector& HitTarget, TArray<FVector_NetQuantize>& OutHitTargets)
{
	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName("MuzzleFlash");
	if(!MuzzleFlashSocket) return;

	const FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
	const FVector TraceStart = SocketTransform.GetLocation();

	const FVector ToTargetNormalize = (HitTarget - TraceStart).GetSafeNormal();
	const FVector SphereCenter = TraceStart + ToTargetNormalize * DistanceToSphere;

	for(uint32 i = 0; i < NumOfPellets; i++)
	{
		const FVector RandVec = UKismetMathLibrary::RandomUnitVector() * FMath::FRandRange(0.0f, SphereRadius);
		const FVector EndLoc = SphereCenter + RandVec;
		FVector ToEndLoc = EndLoc - TraceStart;
		ToEndLoc = TraceStart + ToEndLoc * TRACE_LENGTH / ToEndLoc.Size();

		OutHitTargets.Emplace(ToEndLoc);
	}
}
