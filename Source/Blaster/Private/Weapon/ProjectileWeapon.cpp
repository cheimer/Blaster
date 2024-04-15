// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ProjectileWeapon.h"

#include "Engine/SkeletalMeshSocket.h"
#include "Weapon/Projectile.h"

void AProjectileWeapon::Fire(const FVector& HitTarget)
{
	Super::Fire(HitTarget);

	APawn* InstigatorPawn = Cast<APawn>(GetOwner());

	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName(FName("MuzzleFlash"));
	if(MuzzleFlashSocket && GetWorld())
	{
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		FVector ToTarget = HitTarget - SocketTransform.GetLocation();
		FRotator TargetRotation = ToTarget.Rotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = InstigatorPawn;

		AProjectile* SpawnedProjectile = nullptr;

		if(bUseServerSideRewind)
		{
			if(InstigatorPawn->HasAuthority()) // server
			{
				if(InstigatorPawn->IsLocallyControlled()) // server, host - use replicated projectile
				{
					SpawnedProjectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass,
						SocketTransform.GetLocation(), TargetRotation, SpawnParams);
					SpawnedProjectile->bUseServerSideRewind = false;
					SpawnedProjectile->Damage = Damage;
				}
				else // server, not locally controlled - spawn non-replicated, no SSR
				{
					SpawnedProjectile = GetWorld()->SpawnActor<AProjectile>(ServerSideRewindProjectileClass,
						SocketTransform.GetLocation(), TargetRotation, SpawnParams);
					SpawnedProjectile->bUseServerSideRewind = true;
				}
			}
			else // client, using SSR
			{
				if(InstigatorPawn->IsLocallyControlled()) // client, controlled - spawn non-replicated projectile, use SSR
				{
					SpawnedProjectile = GetWorld()->SpawnActor<AProjectile>(ServerSideRewindProjectileClass,
						SocketTransform.GetLocation(), TargetRotation, SpawnParams);
					SpawnedProjectile->bUseServerSideRewind = true;
					SpawnedProjectile->TraceStart = SocketTransform.GetLocation();
					SpawnedProjectile->InitialVelocity = SpawnedProjectile->GetActorForwardVector() * SpawnedProjectile->InitialSpeed;
					SpawnedProjectile->Damage = Damage;
				}
				else // client, not locally controlled - spawn non-replicated, no SSR
				{
					SpawnedProjectile = GetWorld()->SpawnActor<AProjectile>(ServerSideRewindProjectileClass,
						SocketTransform.GetLocation(), TargetRotation, SpawnParams);
					SpawnedProjectile->bUseServerSideRewind = false;
				}

			}
		}
		else // weapon not using SSR
		{
			if(InstigatorPawn->HasAuthority())
			{
				SpawnedProjectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass,
						SocketTransform.GetLocation(), TargetRotation, SpawnParams);
				SpawnedProjectile->bUseServerSideRewind = false;
				SpawnedProjectile->Damage = Damage;
			}
		}
	}

}
