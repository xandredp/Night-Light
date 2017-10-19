// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseTorch.h"
#include "Core/No_Bark_Vs.h"
#include "Player/NBCharacter.h"
#include "Player/PlayController.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SpotLightComponent.h"
//#include "Components/SceneComponent.h"
#include "Monsters/Base/NBBaseAI.h"
#include "Monsters/Monster Types/NBSkinnyAI.h"

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
	TorchSpotlight->SetAttenuationRadius(2000);
	TorchSpotlight->SetOuterConeAngle(33.0);
	TorchSpotlight->SetInnerConeAngle(22.0);


	StunIntensity = 80000.0f;
	StunAttenuationRadious = 2000.0f;
	MaxIntensity = 8000.0f;
	MaxEnergy = 100.0f;
	CurrentEnergy = MaxEnergy;
	MaxUseDistance = 600.0f;
	CurrentUseDistance = MaxUseDistance;
	EnergyReductionRate = 1.0f;
	ReductionTimerRate = 0.5f;
	EnergyReductionOnPowerUse = 20.0f;
	EnergyIncreaseOnBatteryPickUp = 40.0f;
	EnergyIncreaseOnCrank = 5.0;
	IsEnemySeen = false;
	isFlashed = false;
}

//Called every frame
void ABaseTorch::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);


}

// Called when the game starts or when spawned
void ABaseTorch::BeginPlay()
{
	Super::BeginPlay();

	TorchOnOff(true);	
}

class ANBCharacter * ABaseTorch::GetPawnOwner() const
{
	return MyPawn;
}

void ABaseTorch::SetOwningPawn(ANBCharacter* NewOwner)
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
	//	GetWorldTimerManager().ClearTimer(StartReducingEnergyTimerHandle);
		CurrentEnergy = 0;
	}
	else
	{
		//CurrentEnergy = CurrentEnergy - EnergyReductionRate;
	}
	SetTorchIntensity();
}

UAudioComponent * ABaseTorch::PlayStunSound(USoundCue* SoundToPlay)
{
	UAudioComponent* TorchStunAC = nullptr;

 	if (SoundToPlay && MyPawn)
 	{
		TorchStunAC = UGameplayStatics::SpawnSoundAttached(SoundToPlay, MyPawn->GetRootComponent());
		MakeNoise(100, GetPawnOwner(), GetActorLocation());
	}

	return TorchStunAC;
}

void ABaseTorch::Play2DSound(USoundCue * SoundToPlay)
{
	UGameplayStatics::PlaySound2D(GetWorld(), SoundToPlay);
}

void ABaseTorch::DecreaseEnergy()
{
	if (CurrentEnergy - EnergyReductionOnPowerUse >= 0)
	{
		CurrentEnergy = CurrentEnergy - EnergyReductionOnPowerUse;
	}

	SetTorchIntensity();
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

void ABaseTorch::IncreaseEnergyByFloat(float EnergyToAdd)
{
	CurrentEnergy = CurrentEnergy + EnergyToAdd;
	if (CurrentEnergy >= MaxEnergy)
	{
		CurrentEnergy = MaxEnergy;
	}
	SetTorchIntensity();
}

void ABaseTorch::ActivateTorch()
{
	if (IsEnemySeen)
	{
		if (EnemyPawn)
		{
			//there is enough energy
			if (CurrentEnergy - EnergyReductionOnPowerUse >= 0)
			{
				isFlashed = true;
		
				if (isFlashed)
				{
					PlayStunSound(StunSound);
					TorchSpotlight->SetIntensity(StunIntensity);
					TorchSpotlight->SetAttenuationRadius(StunAttenuationRadious);
					TorchSpotlight->SetOuterConeAngle(33.0);
					isFlashed = false;
				}
				EnemyPawn->OnStun();
				DecreaseEnergy();
			}

			// if we don't have energy 
			else
			{
				// say warning. 
			}
		}
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
	//	GetWorldTimerManager().SetTimer(StartReducingEnergyTimerHandle, this, &ABaseTorch::DrainTorchEnergy, ReductionTimerRate, true);
		SetTorchIntensity();
	}
	else
	{
	//	GetWorldTimerManager().ClearTimer(StartReducingEnergyTimerHandle);
	}

	IsTorchOn = bSpotLightVisiblity;
}

bool ABaseTorch::GetTorchOnOff()
{
	return IsTorchOn;
}
