// Fill out your copyright notice in the Description page of Project Settings.

#include "No_Bark_Vs.h"
#include "NBCharacter.h"
#include "BaseWeapon.h"

ABaseWeapon::ABaseWeapon()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);
	WeaponMesh->SetVisibility(true);

	WeaponCollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollisionComp"));
	WeaponCollisionComp->SetupAttachment(WeaponMesh);

	
}


void ABaseWeapon::FireAmmos()
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

void ABaseWeapon::Instant_Fire()
{
	const int32 RandomSeed = FMath::Rand();
	FRandomStream WeaponRandomStream(RandomSeed);
	const float CurrentSpread = WeaponConfig.WeaponSpread;
	const float SpreadCone = FMath::DegreesToRadians(WeaponConfig.WeaponSpread * 0.5);
	const FVector AimDir = WeaponMesh->GetSocketRotation("MuzzleTip").Vector();
	const FVector StartTrace = WeaponMesh->GetSocketLocation("MuzzleTip");
	const FVector ShootDir = WeaponRandomStream.VRandCone(AimDir, SpreadCone, SpreadCone);
	const FVector EndTrace = StartTrace + ShootDir * WeaponConfig.WeaponRange;
	const FHitResult Impact = WeaponTrace(StartTrace, EndTrace);

	ProcessInstantHit(Impact, StartTrace, ShootDir, RandomSeed, CurrentSpread);
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
	TraceParams.AddIgnoredActor(this);

	FHitResult Hit(ForceInit);

	GetWorld()->LineTraceSingleByChannel(Hit, TraceFrom, TraceTo, COLLISION_INTERACTABLE, TraceParams);

	return Hit;
}

void ABaseWeapon::ProcessInstantHit(const FHitResult & Impact, const FVector & Origin, const FVector & ShootDir, int32 RandomSeed, float ReticleSpread)
{

	const FVector EndTrace = Origin + ShootDir * WeaponConfig.WeaponRange;
	const FVector EndPoint = Impact.GetActor() ? Impact.ImpactPoint : EndTrace;
	DrawDebugLine(this->GetWorld(), Origin, Impact.TraceEnd, FColor::Red, true, 10000, 10.f);

	//AEnemy *Enemy = Cast<AEnemy>(Impact.GetActor());
	//if (Enemy)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "YOU HIT AN ENEMY!!");
	//	Enemy->Destroy();
	//}
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
