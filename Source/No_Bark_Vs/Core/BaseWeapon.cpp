// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/No_Bark_Vs.h"
#include "Player/NBCharacter.h"
#include "Player/PlayController.h"
#include "Items/BaseImpactEffect.h"
#include "NBDamageType.h"
#include "Monsters/Base/Monster.h"
#include "Core/BaseWeapon.h"

ABaseWeapon::ABaseWeapon()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);
	WeaponMesh->SetVisibility(true);
	WeaponMesh->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	WeaponMesh->bReceivesDecals = true;
	WeaponMesh->CastShadow = true;
	WeaponMesh->SetCollisionObjectType(ECC_WorldDynamic);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	WeaponMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	WeaponCollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollisionComp"));
	WeaponCollisionComp->SetupAttachment(WeaponMesh);

	WeaponSpotlight = CreateDefaultSubobject<USpotLightComponent>(TEXT("WeaponSpotlight"));
	WeaponSpotlight->SetupAttachment(WeaponMesh);
	WeaponSpotlight->SetVisibility(true);

	WeaponSpotlight->SetRelativeRotation(FRotator(0, 90, 0));
	WeaponSpotlight->SetRelativeLocation(FVector(0, 30, -10));

	WeaponSpotlight->SetIntensity(1000);
	WeaponSpotlight->SetAttenuationRadius(1000);
	WeaponSpotlight->SetOuterConeAngle(44);

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;

	WeaponConfig.WeaponDamage = 20;
	CurrentState = EWeaponState::Idle;
	TrailTargetParam = "EndPoint";
	MuzzleAttachPoint = "MuzzleTip";

	EquipAnimDuration = 0.5f;
	FireAnimDuration = 1.5f;
	ReloadAnimDuration = 1.1f;
	WeaponConfig.TimeBetweenShots = 0.1f;

	MaxUseDistance = 300;
}

void ABaseWeapon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	FVector camLoc;
	FRotator camRot;
	


	GetPawnOwner()->GetController()->GetPlayerViewPoint(camLoc, camRot);

	const FVector start_trace = camLoc;
	const FVector direction = camRot.Vector();
	const FVector end_trace = start_trace + (direction * MaxUseDistance);

	FCollisionQueryParams TraceParams(FName(TEXT("")), true, this);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.bTraceComplex = true;

	FHitResult Hit(ForceInit);
		
	//Object query parameters
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.ObjectTypesToQuery;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

	//Raycasting in a sphere to detect collisions
	TArray<FHitResult> HitResults;

	//Setting up the shape of the raycast
	FCollisionShape CollisionShape;
	CollisionShape.ShapeType = ECollisionShape::Sphere;
	CollisionShape.SetSphere(10);

	//Handling ignored actors
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->SweepMultiByObjectType(HitResults, start_trace, end_trace, FQuat::Identity, ObjectQueryParams, CollisionShape, QueryParams);
	//Checking for possible hits
	if (bHit)
	{
		for (auto It = HitResults.CreateIterator(); It; It++)
		{
			AMonster* Char = Cast<AMonster>(It->GetActor());
			if (Char)
			{
				FString monsterName;
				monsterName = Char->GetName();
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, monsterName + TEXT(" - hit by sweep!"));
				Char->OnFlashed(GetPawnOwner());
			}
			else
			{

			}
		}
	}
}


class ANBCharacter* ABaseWeapon::GetPawnOwner() const
{
	return MyPawn;
}

void ABaseWeapon::SetTimerForFiring()
{
	Fire();
}
void ABaseWeapon::StopTimerForFiring()
{
	GetWorldTimerManager().ClearTimer(FiringTimerHandle);
}
void ABaseWeapon::FireBullets()
{
	if (CurrentAmmo > 0)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, TEXT("Bullet"));
		Instant_Fire();

		CurrentAmmo -= WeaponConfig.ShotCost;
	}
	else
	{
		ReloadAmmo();
	}
}
void ABaseWeapon::Fire()
{
	if (ProjectileType == EProjectileType::EBullet)
	{
		GetWorldTimerManager().SetTimer(FiringTimerHandle, this, &ABaseWeapon::FireBullets, WeaponConfig.TimeBetweenShots, true);
	}
	if (ProjectileType == EProjectileType::ESpread)
	{
		if (CurrentAmmo > 0)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, TEXT("Spread"));
			for (int32 i = 0; i <= WeaponConfig.WeaponSpread; i++)
			{
				Instant_Fire();
			}

			
			CurrentAmmo -= WeaponConfig.ShotCost;

			// Signal a gunshot
			MakeNoise(100, GetPawnOwner(), GetActorLocation());
		}
		else
		{
			ReloadAmmo();
		}
	}
	if (ProjectileType == EProjectileType::EProjectile)
	{
		if (CurrentAmmo > 0)
		{
		//	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, TEXT("Projectile"));
			ProjectileFire();

			CurrentAmmo -= WeaponConfig.ShotCost;

			// Signal a gunshot
			MakeNoise(100, GetPawnOwner(), GetActorLocation());
		}
		else
		{
			ReloadAmmo();
		}
	}
}

//void ABaseWeapon::Instant_Fire()
//{
//	const int32 RandomSeed = FMath::Rand();
//	FRandomStream WeaponRandomStream(RandomSeed);
//	const float CurrentSpread = WeaponConfig.WeaponSpread;
//	const float SpreadCone = FMath::DegreesToRadians(WeaponConfig.WeaponSpread * 0.5);
//
//	//Shoot from Gun Muzzle
////	const FVector AimDir = WeaponMesh->GetSocketRotation("MuzzleTip").Vector();
//	const FVector StartTrace = WeaponMesh->GetSocketLocation("MuzzleTip");
//
//	//Shoot from Gun Muzzle to screen Cross Direction
//	const FVector AimDir = GetAdjustedAim();
//
//	const FVector ShootDir = WeaponRandomStream.VRandCone(AimDir, SpreadCone, SpreadCone);
//	const FVector EndTrace = StartTrace + ShootDir * WeaponConfig.WeaponRange;
//	const FHitResult Impact = WeaponTrace(StartTrace, EndTrace);
//
//	ProcessInstantHit(Impact, StartTrace, ShootDir, RandomSeed, CurrentSpread);
//
//}
void ABaseWeapon::Instant_Fire()
{
	SimulateWeaponFire();

	const int32 RandomSeed = FMath::Rand();
	FRandomStream WeaponRandomStream(RandomSeed);
	const float CurrentSpread = WeaponConfig.WeaponSpread;
	const float SpreadCone = FMath::DegreesToRadians(WeaponConfig.WeaponSpread * 0.5);
	const FVector MuzzleOrigin = WeaponMesh->GetSocketLocation(MuzzleAttachPoint);
	const FVector AimDir = GetAdjustedAim();
	const FVector CameraPos = GetCameraDamageStartLocation(AimDir);
	
	FVector AdjustedAimDir = WeaponRandomStream.VRandCone(AimDir, SpreadCone, SpreadCone);

	const FVector EndPos = CameraPos + (AdjustedAimDir *  WeaponConfig.WeaponRange);

		/* Check for impact by tracing from the camera position */
	FHitResult Impact = WeaponTrace(CameraPos, EndPos);

	//const FVector ShootDir = WeaponRandomStream.VRandCone(AimDir, SpreadCone, SpreadCone);
	if (Impact.bBlockingHit)
	{
		/* Adjust the shoot direction to hit at the crosshair. */
		AdjustedAimDir = (Impact.ImpactPoint - MuzzleOrigin).GetSafeNormal();
		/* Re-trace with the new aim direction coming out of the weapon muzzle */
		Impact = WeaponTrace(MuzzleOrigin, MuzzleOrigin + (AdjustedAimDir *  WeaponConfig.WeaponRange));
	}
	else
	{
		/* Use the maximum distance as the adjust direction */
		Impact.ImpactPoint = FVector_NetQuantize(EndPos);
	}

	ProcessInstantHit(Impact, MuzzleOrigin, AdjustedAimDir, RandomSeed, CurrentSpread);
}

FVector ABaseWeapon::GetAdjustedAim() const
{
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

	return FinalAim;
}


FVector ABaseWeapon::GetCameraDamageStartLocation(const FVector& AimDir) const
{
	APlayController* PC = MyPawn ? Cast<APlayController>(MyPawn->Controller) : nullptr;
	FVector OutStartTrace = FVector::ZeroVector;

	if (PC)
	{
		FRotator DummyRot;
		PC->GetPlayerViewPoint(OutStartTrace, DummyRot);

		// Adjust trace so there is nothing blocking the ray between the camera and the pawn, and calculate distance from adjusted start
		OutStartTrace = OutStartTrace + AimDir * (FVector::DotProduct((Instigator->GetActorLocation() - OutStartTrace), AimDir));
	}

	return OutStartTrace;
}
void ABaseWeapon::ProjectileFire()
{
}

FHitResult ABaseWeapon::WeaponTrace(const FVector & TraceFrom, const FVector & TraceTo) const
{

	static FName WeaponFireTag = FName(TEXT("WeaponTrace"));

	FCollisionQueryParams TraceParams(WeaponFireTag, true, Instigator);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;
	//TraceParams.AddIgnoredActor(this);

	FHitResult Hit(ForceInit);

	GetWorld()->LineTraceSingleByChannel(Hit, TraceFrom, TraceTo, WEAPON_TRACE, TraceParams);

	return Hit;
}

void ABaseWeapon::ProcessInstantHit(const FHitResult & Impact, const FVector & Origin, const FVector & ShootDir, int32 RandomSeed, float ReticleSpread)
{
	float ActualHitDamage = WeaponConfig.WeaponDamage;
	//const FVector EndTrace = Origin + ShootDir * WeaponConfig.WeaponRange;
	//const FVector EndPoint = Impact.GetActor() ? Impact.ImpactPoint : EndTrace;

	//DrawDebugLine(this->GetWorld(), Origin, Impact.TraceEnd, FColor::Red, true, 1.0f, 0.0f, 1.0f);

//	UNBDamageType* DmgType = Cast<UNBDamageType>(DamageType->GetDefaultObject());
	UPhysicalMaterial * PhysMat = Impact.PhysMaterial.Get();
	AMonster *Enemy = Cast<AMonster>(Impact.GetActor());
	float CurrentDamage = 0;
//	if (PhysMat && DmgType)
	if (PhysMat)
	{
		if (PhysMat->SurfaceType == SURFACE_ENEMYHEAD)
		{
			CurrentDamage = WeaponConfig.WeaponDamage * 2.0f;
		//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "YOU HIT A Head!!");
			Enemy->ReduceHealth(CurrentDamage);
			Enemy->OnShot(GetPawnOwner());
		}
		else if (PhysMat->SurfaceType == SURFACE_ENEMYLIMB)
		{
			CurrentDamage = WeaponConfig.WeaponDamage* 0.5f;
		//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "YOU HIT A Limb!!");
			Enemy->ReduceHealth(CurrentDamage);
			Enemy->OnShot(GetPawnOwner());
		}
		else if (PhysMat->SurfaceType == SURFACE_ENEMYBODY)
		{
			CurrentDamage = WeaponConfig.WeaponDamage;
		//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "YOU HIT A BODY!!");
			Enemy->ReduceHealth(CurrentDamage);
			Enemy->OnShot(GetPawnOwner());
		}
		else if (PhysMat->SurfaceType == SURFACE_FLESH)
		{
		//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "YOU HIT A FLESH!!");
		}
		else if (PhysMat->SurfaceType == SURFACE_DEFAULT)
		{
		//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "YOU HIT A DEFAULT!!");
		}
		else
		{
		//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "What Did you hit!!");
		}
	}

	VisualInstantHit(Impact.ImpactPoint);
}

void ABaseWeapon::SetOwningPawn(ANBCharacter * NewOwner)
{
	if (MyPawn != NewOwner)
	{
		Instigator = NewOwner;
		MyPawn = NewOwner;
	}
}

void ABaseWeapon::AttachToPlayer()
{
	if (MyPawn)
	{
		DetachFromPlayer();

		USkeletalMeshComponent *Character = MyPawn->GetMesh();
		WeaponMesh->SetHiddenInGame(false);
		WeaponMesh->SetupAttachment(Character, "Weapon_Socket");
	}
}

void ABaseWeapon::DetachFromPlayer()
{
	WeaponMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	WeaponMesh->SetHiddenInGame(true);
}

void ABaseWeapon::OnEquip()
{
	WeaponCollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttachToPlayer();
}

void ABaseWeapon::OnUnEquip()
{
	DetachFromPlayer();
}

void ABaseWeapon::ReloadAmmo()
{
	if (CurrentAmmo < WeaponConfig.MaxAmmo)
	{
		if (CurrentClip > 0)
		{
			//palyannimation
			if (ReloadAnimation)
			{
				float AnimDuration = PlayWeaponAnimation(ReloadAnimation);
				if (AnimDuration <= 0.0f)
				{
					AnimDuration = ReloadAnimDuration;
				}
				GetWorldTimerManager().SetTimer(RelaodingTimerHandle, this, &ABaseWeapon::StopReloading, AnimDuration, false);
			}

			PlayWeaponSound(ReloadSound);

			// fill up the current ammo
			float AmmoRemainedInCurrentClip = CurrentAmmo;
			float AddedClipNumbers;
			float ResultClip = CurrentClip + AmmoRemainedInCurrentClip;
			// if there are plenty clips in my gun
			if (ResultClip > WeaponConfig.MaxAmmo)
			{
				CurrentAmmo = WeaponConfig.MaxAmmo;
				AddedClipNumbers = (CurrentClip - WeaponConfig.MaxAmmo) + AmmoRemainedInCurrentClip;
			}
			// if there are no plenty clips in my gun. add all clips into gun
			else
			{
				CurrentAmmo = CurrentClip + AmmoRemainedInCurrentClip;
				AddedClipNumbers = CurrentClip; 
			}

			CurrentClip = CurrentClip - AddedClipNumbers;
		}
		
	}
	
}
void ABaseWeapon::StopReloading()
{
	GetWorldTimerManager().ClearTimer(RelaodingTimerHandle);
	StopWeaponAnimation(ReloadAnimation);
}
void ABaseWeapon::SimulateWeaponFire()
{
	if (MuzzleFX)
	{
	
		MuzzlePSC = UGameplayStatics::SpawnEmitterAttached(MuzzleFX, WeaponMesh, MuzzleAttachPoint);
		MuzzlePSC->AddWorldRotation(FRotator(0.0f, 90.0f, 0.0f));
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "FXFXSpwned..");
	}

	if (!bPlayingFireAnim)
	{
		PlayWeaponAnimation(FireAnimation);
		bPlayingFireAnim = true;
	}

	PlayWeaponSound(FireSound);
}

void ABaseWeapon::StopSimulatingWeaponFire()
{
	if (bPlayingFireAnim)
	{
		StopWeaponAnimation(FireAnimation);
		bPlayingFireAnim = false;
	}
}

FVector ABaseWeapon::GetMuzzleLocation() const
{
	return FVector();
}

FVector ABaseWeapon::GetMuzzleDirection() const
{
	return FVector();
}

UAudioComponent * ABaseWeapon::PlayWeaponSound(USoundCue * Sound)
{
	UAudioComponent* WeaponAC = nullptr;

	if (Sound && MyPawn)
	{
		WeaponAC = UGameplayStatics::SpawnSoundAttached(Sound, MyPawn->GetRootComponent());
		// Signal a gunshot
		MakeNoise(100, GetPawnOwner(), GetActorLocation());

	}
	return WeaponAC;
}

float ABaseWeapon::PlayWeaponAnimation(UAnimMontage * Animation, float InPlayRate, FName StartSectionName)
{
	float Duration = 0.0f;
	if (MyPawn)
	{
		if (Animation)
		{
			Duration = MyPawn->PlayAnimMontage(Animation, InPlayRate, StartSectionName);
		}
	}

	return Duration;
}

void ABaseWeapon::StopWeaponAnimation(UAnimMontage * Animation)
{
	if (MyPawn)
	{
		if (Animation)
		{
			MyPawn->StopAnimMontage(Animation);
		}
	}
}

void ABaseWeapon::VisualInstantHit(const FVector& ImpactPoint)
{
	/* Adjust direction based on desired crosshair impact point and muzzle location */
	const FVector MuzzleOrigin = WeaponMesh->GetSocketLocation(MuzzleAttachPoint);
	const FVector AimDir = (ImpactPoint - MuzzleOrigin).GetSafeNormal();

	const FVector EndTrace = MuzzleOrigin + (AimDir *WeaponConfig.WeaponRange);
	const FHitResult Impact = WeaponTrace(MuzzleOrigin, EndTrace);

	if (Impact.bBlockingHit)
	{
	//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "IF Visual InstantHit!");
		VisualImpactEffects(Impact);
		//VisualTrailEffects(Impact.ImpactPoint);
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "ELSE Visual InstantHit!");
	//	VisualTrailEffects(EndTrace);
	}
}


void ABaseWeapon::VisualImpactEffects(const FHitResult& Impact)
{
	
	if (ImpactTemplate && Impact.bBlockingHit)
	{
	//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "IF Visual InstantHit!");
		// TODO: Possible re-trace to get hit component that is lost during replication.

		/* This function prepares an actor to spawn, but requires another call to finish the actual spawn progress. This allows manipulation of properties before entering into the level */
		ABaseImpactEffect* EffectActor = GetWorld()->SpawnActorDeferred<ABaseImpactEffect>(ImpactTemplate, FTransform(Impact.ImpactPoint.Rotation(), Impact.ImpactPoint));
		if (EffectActor)
		{
			EffectActor->SurfaceHit = Impact;
			UGameplayStatics::FinishSpawningActor(EffectActor, FTransform(Impact.ImpactNormal.Rotation(), Impact.ImpactPoint));
		}
	}
}


void ABaseWeapon::VisualTrailEffects(const FVector& EndPoint)
{
	const FVector MuzzleOrigin = WeaponMesh->GetSocketLocation(MuzzleAttachPoint);
//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "TRAILEFFECTWORKING!");
	// Keep local count for effects
	BulletsShotCount++;
	FVector ShootDir = EndPoint - MuzzleOrigin;

	// Only spawn if a minimum distance is satisfied.
	if (ShootDir.Size() < MinimumProjectileSpawnDistance)
	{
		return;
	}

	if (BulletsShotCount % TracerRoundInterval == 0)
	{
		if (TracerFX)
		{
			ShootDir.Normalize();
			UGameplayStatics::SpawnEmitterAtLocation(this, TracerFX, MuzzleOrigin, ShootDir.Rotation());
		}
	}
	else
	{
		// Only create trails FX by other players.
		ANBCharacter* OwningPawn = GetPawnOwner();
		if (OwningPawn && OwningPawn->IsLocallyControlled())
		{
			return;
		}

		if (TrailFX)
		{
			UParticleSystemComponent* TrailPSC = UGameplayStatics::SpawnEmitterAtLocation(this, TrailFX, MuzzleOrigin);
			if (TrailPSC)
			{
				TrailPSC->SetVectorParameter(TrailTargetParam, EndPoint);
			}
		}
	}
}


void ABaseWeapon::TurnOnTorch()
{
	WeaponSpotlight->SetVisibility(true);
}


void ABaseWeapon::TurnOffTorch()
{
	WeaponSpotlight->SetVisibility(false);
}