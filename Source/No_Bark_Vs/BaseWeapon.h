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
UENUM()
enum class EWeaponState : uint8
{
	Idle,
	Firing,
	Equipping,
	Reloading
};
UENUM()
enum class EAttackType : uint8
{
	/* For currently equipped items/weapons */
	EMelee,

	/* For primary weapons on spine bone */
	ERanged,

};

UENUM(BlueprintType)
enum EProjectileType
{
	EBullet			UMETA(DisplayName = "Bullet"),
	ESpread			UMETA(DisplayName = "Spread"),
	EProjectile	UMETA(DisplayName = "Projectile"),

};

//TableRowBase for Weapon Data TABLE. 
USTRUCT(BlueprintType)
struct FWeaponData 
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EAttackType eAttackType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
		float WeaponDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
		float WeaponRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
		float WeaponSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
		float WeaponSpread;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo)
		int32 MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
		int32 MaxClip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
		float TimeBetweenShots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo)
		int32 ShotCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
		FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
		UTexture2D* SplashArt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
		int32 Priority;

};


UCLASS()
class NO_BARK_VS_API ABaseWeapon : public AActor
{
	GENERATED_BODY()



public:
	ABaseWeapon();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Owning")
		class ANBCharacter* MyPawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collision")
		class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collision")
		class UBoxComponent* WeaponCollisionComp;


	UFUNCTION(BlueprintCallable, Category = "Config")
	void Fire();
	
	UFUNCTION(BlueprintCallable, Category = "Config")
	void Instant_Fire();

	UFUNCTION(BlueprintCallable, Category = "Config")
		virtual void ProjectileFire();

	//WeaponInfo
	UPROPERTY(EditDefaultsOnly, Category = Config)
		FWeaponData WeaponConfig;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
		TEnumAsByte<EProjectileType> ProjectileType;

	UPROPERTY(EditDefaultsOnly, Category = Config)
		USoundCue *FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
		int32 CurrentAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
		int32 CurrentClip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
		EWeaponState CurrentState;
	//damage

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UNBDamageType> DamageType;


	/* Set the weapon's owning pawn */
	void SetOwningPawn(ANBCharacter* NewOwner);

	/* Get pawn owner */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		class ANBCharacter* GetPawnOwner() const;

	void AttachToPlayer();
	void DetachFromPlayer();

	void OnEquip();
	void OnUnEquip();

	void ReloadAmmo();

	UAudioComponent* PlayWeaponSound(USoundCue *Sound);


protected:
	FHitResult WeaponTrace(const FVector &TraceFrom, const FVector &TraceTo) const;

	void ProcessInstantHit(const FHitResult &Impact, const FVector &Origin, const FVector &ShootDir, int32 RandomSeed, float ReticleSpread);

	FVector GetAdjustedAim() const;
	FVector GetCameraDamageStartLocation(const FVector& AimDir) const;



};
