// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickup.h"
#include "Engine/DataTable.h"
#include "BaseWeapon.generated.h"

class ANBCharacter;

/**
 * 
 */
//
UENUM(BlueprintType)
enum EProjectileType
{
	EBullet			UMETA(DisplayName = "Bullet"),
	ESpread			UMETA(DisplayName = "Spread"),
	EProjectile	UMETA(DisplayName = "Projectile"),

};

//TableRowBase for Weapon Data TABLE. 
USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase
{
	GENERATED_BODY()

public:


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo)
		int32 MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
		int32 MaxClip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
		float TimeBetweenShots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo)
		int32 ShotCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
		float WeaponRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
		float WeaponSpread;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
		FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
		UTexture2D* SplashArt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
		int32 Priority;
};


UCLASS()
class NO_BARK_VS_API ABaseWeapon : public APickup
{
	GENERATED_BODY()

public:
	ABaseWeapon();


	UFUNCTION(BlueprintCallable, Category = "Config")
	void FireAmmos();
	
	UFUNCTION(BlueprintCallable, Category = "Config")
	void Instant_Fire();

	UFUNCTION(BlueprintCallable, Category = "Config")
		virtual void ProjectileFire();

	UPROPERTY(EditDefaultsOnly, Category = "Config")
		FWeaponData WeaponConfig;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
		TEnumAsByte<EProjectileType> ProjectileType;

	UPROPERTY(EditDefaultsOnly, Category = Config)
		USoundCue *FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
		int32 CurrentAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
		int32 CurrentClip;

	void SetOwningPawn(ANBCharacter *NewOwner);

	void AttachToPlayer();
	void DetachFromPlayer();

	void OnEquip();
	void OnUnEquip();

	void ReloadAmmo();

	UAudioComponent* PlayWeaponSound(USoundCue *Sound);


protected:
	FHitResult WeaponTrace(const FVector &TraceFrom, const FVector &TraceTo) const;

	void ProcessInstantHit(const FHitResult &Impact, const FVector &Origin, const FVector &ShootDir, int32 RandomSeed, float ReticleSpread);

	ANBCharacter *MyPawn;

};
