// Fill out your copyright notice in the Description page of Project Settings.


#include "CaptureFlag/FlagZone.h"

#include "Character/BlasterCharacter.h"
#include "Components/SphereComponent.h"
#include "GameMode/CaptureFlagGameMode.h"
#include "Weapon/Flag.h"

AFlagZone::AFlagZone()
{
	PrimaryActorTick.bCanEverTick = false;

	ZoneSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ZoneSphere"));
	SetRootComponent(ZoneSphere);

}

void AFlagZone::BeginPlay()
{
	Super::BeginPlay();

	ZoneSphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereOverlap);
}

void AFlagZone::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AFlag* OverlappingFlag = Cast<AFlag>(OtherActor);
	if(OverlappingFlag && OverlappingFlag->GetTeam() != Team)
	{
		ACaptureFlagGameMode* GameMode = GetWorld()->GetAuthGameMode<ACaptureFlagGameMode>();
		if(GameMode)
		{
			GameMode->FlagCaptured(OverlappingFlag, this);
		}
		OverlappingFlag->ResetFlag();
	}
}
