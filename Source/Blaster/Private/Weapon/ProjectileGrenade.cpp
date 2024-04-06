// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ProjectileGrenade.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

AProjectileGrenade::AProjectileGrenade()
{
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>("GrenadeMesh");
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("RocketMovementComponent"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->SetIsReplicated(true);
	ProjectileMovementComponent->bShouldBounce = true;
}

void AProjectileGrenade::BeginPlay()
{
	//Super::BeginPlay();
	AActor::BeginPlay();

	StartDestroyTimer();
	SpawnTrailSystem();

	ProjectileMovementComponent->OnProjectileBounce.AddDynamic(this, &ThisClass::OnBounce);
}

void AProjectileGrenade::Destroyed()
{
	ExplodeDamage();

	Super::Destroyed();
}

void AProjectileGrenade::OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
	if(BounceSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, BounceSound, GetActorLocation());
	}
}
