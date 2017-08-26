// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BaseImpactEffect.generated.h"

UCLASS()
class NO_BARK_VS_API ABaseImpactEffect : public AActor
{
	GENERATED_BODY()

public:

	ABaseImpactEffect();

	virtual void PostInitializeComponents() override;


	/**** BULLET IMPACT EFFECTS ****/
	UPROPERTY(EditDefaultsOnly)
		class UParticleSystem* DefaultFX;

	UPROPERTY(EditDefaultsOnly)
		class UParticleSystem* MetalFX;

	UPROPERTY(EditDefaultsOnly)
		class UParticleSystem* ConcreteFX;

	UPROPERTY(EditDefaultsOnly)
		class UParticleSystem* WoodFX;

	UPROPERTY(EditDefaultsOnly)
		class UParticleSystem* PlayerFleshFX;

	UPROPERTY(EditDefaultsOnly)
		class UParticleSystem* EnemyFleshFX;

	UPROPERTY(EditDefaultsOnly)
		class UParticleSystem* EnemySparkFX;


	/**** BULLET IMPACT SOUNDS ****/
	UPROPERTY(EditDefaultsOnly)
		class USoundCue* DefaultSound;

	UPROPERTY(EditDefaultsOnly)
		class USoundCue* MetalSound;

	UPROPERTY(EditDefaultsOnly)
		class USoundCue* ConcreteSound;

	UPROPERTY(EditDefaultsOnly)
		class USoundCue* WoodSound;

	UPROPERTY(EditDefaultsOnly)
		class USoundCue* PlayerFleshSound;

	UPROPERTY(EditDefaultsOnly)
		class USoundCue* EnemyFleshSound;

	UPROPERTY(EditDefaultsOnly)
		class USoundCue* EnemySparkSound;

	UPROPERTY(EditDefaultsOnly, Category = "Decal")
		class UMaterial* DecalMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Decal")
		float DecalSize;

	UPROPERTY(EditDefaultsOnly, Category = "Decal")
		float DecalLifeSpan;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		bool IsEnemyInDark;

	FHitResult SurfaceHit;

protected:

	class UParticleSystem* GetImpactFX(EPhysicalSurface SurfaceType) const;

	class USoundCue* GetImpactSound(EPhysicalSurface SurfaceType) const;
};
