// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Flag.h"

#include "Character/BlasterCharacter.h"
#include "Components/SphereComponent.h"

AFlag::AFlag()
{

}

void AFlag::ResetFlag()
{
	ABlasterCharacter* FlagOwner = Cast<ABlasterCharacter>(GetOwner());
	if(FlagOwner)
	{
		FlagOwner->SetHoldingFlag(false);
		FlagOwner->SetOverlappingWeapon(nullptr);
		FlagOwner->UnCrouch();
	}

	if(HasAuthority())
	{
		FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
		GetWeaponMesh()->DetachFromComponent(DetachRules);

		SetWeaponState(EWeaponState::EWS_Initial);
		GetAreaSphere()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetAreaSphere()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

		SetOwner(nullptr);
		BlasterOwnerCharacter = nullptr;
		BlasterOwnerController = nullptr;

		SetActorTransform(InitialTransform);
	}
}

void AFlag::BeginPlay()
{
	Super::BeginPlay();

	InitialTransform = GetActorTransform();
}

void AFlag::OnEquipped()
{
	//Super::OnEquipped();
	ShowPickupWidget(false);

	GetAreaSphere()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetWeaponMesh()->SetSimulatePhysics(false);
	GetWeaponMesh()->SetEnableGravity(false);
	GetWeaponMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetWeaponMesh()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
}

void AFlag::OnDropped()
{
	//Super::OnDropped();
	if(HasAuthority())
	{
		GetAreaSphere()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	GetWeaponMesh()->SetSimulatePhysics(true);
	GetWeaponMesh()->SetEnableGravity(true);
	GetWeaponMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetWeaponMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	GetWeaponMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	GetWeaponMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	GetWeaponMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_BLUE);
	GetWeaponMesh()->MarkRenderStateDirty();
}
