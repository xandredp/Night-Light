// Fill out your copyright notice in the Description page of Project Settings.

#include "No_Bark_Vs.h"
#include "BaseImpactEffect.h"


// Sets default values
ABaseImpactEffect::ABaseImpactEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseImpactEffect::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseImpactEffect::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}



UParticleSystem* ABaseImpactEffect::GetImpactFX(EPhysicalSurface SurfaceType) const
{
	switch (SurfaceType)
	{
	case SURFACE_DEFAULT:
		return DefaultFX;
	case SURFACE_FLESH:
		return PlayerFleshFX;
	case SURFACE_ENEMYBODY:
	case SURFACE_ENEMYHEAD:
	case SURFACE_ENEMYLIMB:
		return ZombieFleshFX;
	default:
		return nullptr;
	}
}


USoundCue* ABaseImpactEffect::GetImpactSound(EPhysicalSurface SurfaceType) const
{
	switch (SurfaceType)
	{
	case SURFACE_DEFAULT:
		return DefaultSound;
	case SURFACE_FLESH:
		return PlayerFleshSound;
	case SURFACE_ENEMYBODY:
	case SURFACE_ENEMYHEAD:
	case SURFACE_ENEMYLIMB:
		return ZombieFleshSound;
	default:
		return nullptr;
	}
}