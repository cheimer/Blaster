// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ProjectileRocket.h"

#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Sound/SoundCue.h"
#include "NiagaraComponent.h"
#include "Components/AudioComponent.h"
#include "Weapon/RocketMovementComponent.h"

AProjectileRocket::AProjectileRocket()
{
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>("RocketMesh");
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	RocketMovementComponent = CreateDefaultSubobject<URocketMovementComponent>(TEXT("RocketMovementComponent"));
	RocketMovementComponent->bRotationFollowsVelocity = true;
	RocketMovementComponent->SetIsReplicated(true);
}

void AProjectileRocket::BeginPlay()
{
	Super::BeginPlay();

	if(!HasAuthority())
	{
		CollisionBox->OnComponentHit.AddDynamic(this, &ThisClass::OnHit);
	}

	SpawnTrailSystem();

	if(ProjectileLoop && LoopingSoundAttenuation)
	{
		ProjectileLoopComponent = UGameplayStatics::SpawnSoundAttached(
			ProjectileLoop,
			GetRootComponent(),
			FName(),
			GetActorLocation(),
			EAttachLocation::KeepWorldPosition,
			false,
			1.0f,
			1.0f,
			0.0f,
			LoopingSoundAttenuation,
			(USoundConcurrency*)nullptr,
			false
		);
	}
}

void AProjectileRocket::Destroyed()
{
	//Super::Destroyed();
}

void AProjectileRocket::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                              UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if(OtherActor == GetOwner())
    {
    	return;
    }
	APawn* FiringPawn = GetInstigator();
	if(FiringPawn && HasAuthority())
	{
		AController* FiringController = FiringPawn->GetController();
		if(FiringController)
		{
			UGameplayStatics::ApplyRadialDamageWithFalloff(
				this,
				Damage,
				10.0f,
				GetActorLocation(),
				200.0f,
				500.0f,
				1.0f,
				UDamageType::StaticClass(),
				TArray<AActor*>(),
				this,
				FiringController
				);
		}
	}
	StartDestroyTimer();

	if(ImpactParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, GetActorTransform());
	}
	if(ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, GetActorLocation());
	}

	if(ProjectileMesh)
	{
		ProjectileMesh->SetVisibility(false);
	}
	if(CollisionBox)
	{
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	if(TrailSystemComponent && TrailSystemComponent->GetSystemInstanceController())
	{
		TrailSystemComponent->GetSystemInstanceController()->Deactivate();
	}
	if(ProjectileLoopComponent && ProjectileLoopComponent->IsPlaying())
	{
		ProjectileLoopComponent->Stop();
	}

	//Super::OnHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
}
