// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup/ShieldPickup.h"

#include "BlasterComponent/BuffComponent.h"
#include "Character/BlasterCharacter.h"

void AShieldPickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor);
	if(BlasterCharacter)
	{
		UBuffComponent* Buff = BlasterCharacter->GetBuff();
		if(Buff)
		{
			Buff->ShieldRepairing(ShieldAmount, ShieldRepairTime);
		}

		Destroy();
	}
}
