// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseTorch.h"
#include "Core/No_Bark_Vs.h"
#include "Player/NBCharacter.h"
#include "Player/PlayController.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SpotLightComponent.h"
//#include "Components/SceneComponent.h"
#include "Monsters/Base/Monster.h"


// Sets default values
ABaseTorch::ABaseTorch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TorchSpotlight = CreateDefaultSubobject<USpotLightComponent>(TEXT("WeaponSpotlight"));
	//TorchSpotlight->SetupAttachment(RootComponent);
	//SetRootComponent(TorchSpotlight);
	TorchSpotlight->SetRelativeRotation(FRotator(0, 0, 0));
	TorchSpotlight->SetRelativeLocation(FVector(0, 0, -0));

	TorchSpotlight->SetIntensity(8000);
	TorchSpotlight->SetAttenuationRadius(7000);
	TorchSpotlight->SetOuterConeAngle(33.0);
	TorchSpotlight->SetInnerConeAngle(22.0);


	StunIntensity = 80000.0f;
	MaxIntensity = 8000.0f;
	MaxEnergy = 100.0f;
	CurrentEnergy = MaxEnergy;
	MaxUseDistance = 600.0f;
	CurrentUseDistance = MaxUseDistance;
	EnergyReductionRate = 1.0f;
	ReductionTimerRate = 0.5f;
	EnergyReductionOnPowerUse = 20.0f;
	EnergyIncreaseOnCrank = 5.0;


}

// Called when the game starts or when spawned
void ABaseTorch::BeginPlay()
{
	Super::BeginPlay();

	TorchOnOff(true);	
}
void ABaseTorch::SetOwningPawn(class ANBCharacter* NewOwner)
{
	if (MyPawn != NewOwner)
	{
		Instigator = NewOwner;
		MyPawn = NewOwner;
	}
}
void ABaseTorch::SetTorchIntensity()
{
	float EnergyRatio = CurrentEnergy / 100;
	if (EnergyRatio > 0.9)
	{
		TorchSpotlight->SetIntensity(MaxIntensity);
	}
	else
	{
		TorchSpotlight->SetIntensity((MaxIntensity/2) * EnergyRatio);
	}
	CurrentUseDistance = MaxUseDistance * EnergyRatio;
}

FHitResult ABaseTorch::TorchLightTrace(const FVector & TraceFrom, const FVector & TraceTo) const
{
	static FName WeaponFireTag = FName(TEXT("WeaponTrace"));

	//Setting up the shape of the raycast
	FCollisionShape CollisionShape;
	CollisionShape.ShapeType = ECollisionShape::Sphere;
	CollisionShape.SetSphere(10);

	FCollisionQueryParams TraceParams(WeaponFireTag, true, Instigator);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;
	//TraceParams.AddIgnoredActor(this);

	FHitResult Hit(ForceInit);

	bool bHit = GetWorld()->SweepSingleByChannel(Hit, TraceFrom, TraceTo, FQuat::Identity, TORCH_TRACE, CollisionShape, TraceParams);

//	GetWorld()->LineTraceSingleByChannel(Hit, TraceFrom, TraceTo, WEAPON_TRACE, TraceParams);

	return Hit;
}

void ABaseTorch::DrainTorchEnergy()
{
	if (CurrentEnergy <= 0)
	{
		GetWorldTimerManager().ClearTimer(StartReducingEnergyTimerHandle);
		CurrentEnergy = 0;
	}
	else
	{
		CurrentEnergy = CurrentEnergy - EnergyReductionRate;
	}
	SetTorchIntensity();
}

void ABaseTorch::DecreaseEnergy()
{
	if (CurrentEnergy - EnergyReductionOnPowerUse >= 0)
	{
		CurrentEnergy = CurrentEnergy - EnergyReductionOnPowerUse;
	}
}

void ABaseTorch::IncreaseEnergy()
{
	CurrentEnergy = CurrentEnergy + EnergyIncreaseOnCrank;
	if (CurrentEnergy >= MaxEnergy)
	{
		CurrentEnergy = MaxEnergy;
	}
	SetTorchIntensity();
}

void ABaseTorch::ActivateTorch()
{
	//if we have enough power
	if (CurrentEnergy - EnergyReductionOnPowerUse >= 0)
	{
		// 1. Change Intensity
		
		// 2. RayCasting

		const FVector start_trace = TorchSpotlight->GetComponentLocation();
		//Get AimDirection
		APlayController* const PC = Instigator ? Cast<APlayController>(Instigator->Controller) : nullptr;
		FVector FinalAim = FVector::ZeroVector;

		if (PC)
		{
			FVector CamLoc;
			FRotator CamRot;
			PC->GetPlayerViewPoint(CamLoc, CamRot);

			FinalAim = CamRot.Vector();
		}
		else if (Instigator)
		{
			FinalAim = Instigator->GetBaseAimRotation().Vector();
		}
		const FVector AimDir = FinalAim;
		const FVector end_trace = start_trace + (AimDir * CurrentUseDistance);

		/* Check for impact by tracing from the camera position */
		FHitResult Impact = TorchLightTrace(start_trace, end_trace);
		DrawDebugLine(GetWorld(), start_trace, end_trace, FColor::Green, false, 10.0, 0, 0.5f);
		// 3. StunEnemy
		ProcessInstantHit(Impact);

		// 3. blueprintimplementable effect In blueprint it will do spawn effect
		//4. DecreaseEnergy
		DecreaseEnergy();
	}

	// if we don't have energy 
	else
	{
		// say warning. 
	}
	TorchOnOff(true);
}



void ABaseTorch::TorchCrank()
{
	IncreaseEnergy();
	
}
void ABaseTorch::TorchOnOff(bool bSpotLightVisiblity)
{
	TorchSpotlight->SetVisibility(bSpotLightVisiblity);
	
	if (bSpotLightVisiblity == true)
	{
		GetWorldTimerManager().SetTimer(StartReducingEnergyTimerHandle, this, &ABaseTorch::DrainTorchEnergy, ReductionTimerRate, true);
		SetTorchIntensity();
	}
	else
	{
		GetWorldTimerManager().ClearTimer(StartReducingEnergyTimerHandle);
	}
}


void ABaseTorch::ProcessInstantHit(const FHitResult & Impact)
{

	UPhysicalMaterial * PhysMat = Impact.PhysMaterial.Get();
	AMonster *Enemy = Cast<AMonster>(Impact.GetActor());
	if (Enemy)
	{
		Enemy->OnFlashed(MyPawn);
	}
}

//	VisualInstantHit(Impact.ImpactPoint);
//}
//
//
//void ABaseWeapon::VisualInstantHit(const FVector& ImpactPoint)
//{
//	/* Adjust direction based on desired crosshair impact point and muzzle location */
//	const FVector MuzzleOrigin = WeaponMesh->GetSocketLocation(MuzzleAttachPoint);
//	const FVector AimDir = (ImpactPoint - MuzzleOrigin).GetSafeNormal();
//
//	const FVector EndTrace = MuzzleOrigin + (AimDir *WeaponConfig.WeaponRange);
//	const FHitResult Impact = WeaponTrace(MuzzleOrigin, EndTrace);
//
//	if (Impact.bBlockingHit)
//	{
//		//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "IF Visual InstantHit!");
//
//		VisualImpactEffects(Impact);
//		//VisualTrailEffects(Impact.ImpactPoint);
//	}
//	else
//	{
//		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "ELSE Visual InstantHit!");
//		//	VisualTrailEffects(EndTrace);
//	}
//}