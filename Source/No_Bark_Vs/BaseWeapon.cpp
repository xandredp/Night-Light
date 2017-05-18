// Fill out your copyright notice in the Description page of Project Settings.

#include "No_Bark_Vs.h"
#include "NBCharacter.h"
#include "PlayController.h"
//#include "BaseEnemy.h"
#include "NBDamageType.h"
#include "Monster.h"
#include "BaseWeapon.h"

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

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;

	WeaponConfig.WeaponDamage = 20;
	CurrentState = EWeaponState::Idle;
}
class ANBCharacter* ABaseWeapon::GetPawnOwner() const
{
	return MyPawn;
}

void ABaseWeapon::Fire()
{

	if (ProjectileType == EProjectileType::EBullet)
	{
		if (CurrentClip > 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, TEXT("Bullet"));
			Instant_Fire();
			//PlayWeaponSound(FireSound);
			//CurrentClip -= WeaponConfig.ShotCost;
		}
		else
		{
			//ReloadAmmo();
		}
	}
	if (ProjectileType == EProjectileType::ESpread)
	{
		if (CurrentClip > 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, TEXT("Spread"));
			for (int32 i = 0; i <= WeaponConfig.WeaponSpread; i++)
			{
				Instant_Fire();
			}
			//PlayWeaponSound(FireSound);
			//CurrentClip -= WeaponConfig.ShotCost;
		}
		else
		{
			//ReloadAmmo();
		}
	}
	if (ProjectileType == EProjectileType::EProjectile)
	{
		if (CurrentClip > 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, TEXT("Projectile"));
			ProjectileFire();
			//PlayWeaponSound(FireSound);
			//CurrentClip -= WeaponConfig.ShotCost;
		}
		else
		{
			//ReloadAmmo();
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
	const int32 RandomSeed = FMath::Rand();
	FRandomStream WeaponRandomStream(RandomSeed);
	const float CurrentSpread = WeaponConfig.WeaponSpread;
	const float SpreadCone = FMath::DegreesToRadians(WeaponConfig.WeaponSpread * 0.5);

	const FVector AimDir = GetAdjustedAim();
	const FVector CameraPos = GetCameraDamageStartLocation(AimDir);
	
	FVector AdjustedAimDir = WeaponRandomStream.VRandCone(AimDir, SpreadCone, SpreadCone);

	const FVector EndPos = CameraPos + (AdjustedAimDir *  WeaponConfig.WeaponRange);
	const FVector MuzzleOrigin = WeaponMesh->GetSocketLocation("MuzzleTip");

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

	DrawDebugLine(this->GetWorld(), Origin, Impact.TraceEnd, FColor::Red, true, 500, 10.f);

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
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "YOU HIT A Head!!");
			Enemy->ReduceHealth(CurrentDamage);
		}
		else if (PhysMat->SurfaceType == SURFACE_ENEMYLIMB)
		{
			CurrentDamage = WeaponConfig.WeaponDamage* 0.5f;
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "YOU HIT A Limb!!");
			Enemy->ReduceHealth(CurrentDamage);
		}
		else if (PhysMat->SurfaceType == SURFACE_ENEMYBODY)
		{
			CurrentDamage = WeaponConfig.WeaponDamage;
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "YOU HIT A BODY!!");
			Enemy->ReduceHealth(CurrentDamage);
		}
		else if (PhysMat->SurfaceType == SURFACE_FLESH)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "YOU HIT A FLESH!!");
		}
		else if (PhysMat->SurfaceType == SURFACE_DEFAULT)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "YOU HIT A DEFAULT!!");
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "What Did you hit!!");
		}

	
	}


	//ABaseEnemy *Enemy = Cast<ABaseEnemy>(Impact.GetActor());
	//if (Enemy)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "YOU HIT AN ENEMY!!");
	//	Enemy->Destroy();
	//}

	//FPointDamageEvent PointDmg;
	////PointDmg.DamageTypeClass = DamageType;
	//PointDmg.HitInfo = Impact;
	//PointDmg.ShotDirection = ShootDir;
	//PointDmg.Damage = ActualHitDamage;

	//Impact.GetActor()->TakeDamage(PointDmg.Damage, PointDmg, MyPawn->Controller, this);
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
}

UAudioComponent * ABaseWeapon::PlayWeaponSound(USoundCue * Sound)
{
	return nullptr;
}
