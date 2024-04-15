// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/HitScanWeapon.h"

#include "BlasterComponent/LagCompensationComponent.h"
#include "Character/BlasterCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "PlayerController/BlasterPlayerController.h"
#include "Sound/SoundCue.h"

void AHitScanWeapon::Fire(const FVector& HitTarget)
{
	Super::Fire(HitTarget);

	if(!GetWorld()) return;

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if(!OwnerPawn) return;

	AController* InstigatorController = OwnerPawn->GetController();

	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName("MuzzleFlash");
	if(MuzzleFlashSocket)
	{
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		FVector Start = SocketTransform.GetLocation();

		FHitResult FireHit;
		WeaponTraceHit(Start, HitTarget, FireHit);

		if(FireHit.bBlockingHit)
		{
			ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(FireHit.GetActor());
			if(BlasterCharacter && InstigatorController)
			{
				bool bCauseAuthDamage = !bUseServerSideRewind || OwnerPawn->IsLocallyControlled();
				if(HasAuthority() && bCauseAuthDamage)
				{
					UGameplayStatics::ApplyDamage(BlasterCharacter, Damage, InstigatorController,
						this,UDamageType::StaticClass());
				}
				else if(!HasAuthority() && bUseServerSideRewind)
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
						BlasterOwnerCharacter->GetLagCompensation()->ServerScoreRequest(BlasterCharacter, Start, HitTarget,
							BlasterOwnerController->GetServerTime() - BlasterOwnerController->SingleTripTime, this);
					}
				}
			}
			if(ImpactParticle)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle,
					FireHit.ImpactPoint, FireHit.ImpactNormal.Rotation());
			}
			if(HitSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, HitSound, FireHit.ImpactPoint);
			}
		}
		if(MuzzleFlash)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, SocketTransform);
		}
		if(FireSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}
	}
}

void AHitScanWeapon::WeaponTraceHit(const FVector& TraceStart, const FVector& HitTarget, FHitResult& OutHit)
{
	if(!GetWorld()) return;

	FVector End = TraceStart + (HitTarget - TraceStart) * 1.25f;

	GetWorld()->LineTraceSingleByChannel(OutHit, TraceStart, End,ECC_Visibility);

	FVector BeamEnd = End;
	if(OutHit.bBlockingHit)
	{
		BeamEnd = OutHit.ImpactPoint;
	}
	if(BeamParticle)
	{
		UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(), BeamParticle, TraceStart, FRotator::ZeroRotator, true);
		if(Beam)
		{
			Beam->SetVectorParameter(FName("Target"), BeamEnd);

		}
	}
}
