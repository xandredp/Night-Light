// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core/Pickup.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
		int32 MaxClip;
	//MaxAmmom Per Clip
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
		int32 MaxAmmo;

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


	//firing between shots
	FTimerHandle FiringTimerHandle;
	//ReloadingTimeHandler
	FTimerHandle RelaodingTimerHandle;
public:
	ABaseWeapon();

	void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Owning")
		class ANBCharacter* MyPawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collision")
		class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collision")
		class UBoxComponent* WeaponCollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Config")
		class USpotLightComponent* WeaponSpotlight;

	UFUNCTION(BlueprintCallable, Category = "Config")
		void SetTimerForFiring();
	UFUNCTION(BlueprintCallable, Category = "Config")
		void StopTimerForFiring();

	UFUNCTION(BlueprintCallable, Category = "Config")
	void Fire();
	
	UFUNCTION(BlueprintCallable, Category = "Config")
		void FireBullets();
	UFUNCTION(BlueprintCallable, Category = "Config")
	void Instant_Fire();

	UFUNCTION(BlueprintCallable, Category = "Config")
		virtual void ProjectileFire();

	//WeaponInfo
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
		FWeaponData WeaponConfig;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
		TEnumAsByte<EProjectileType> ProjectileType;
	// number of total ammo you are carrying
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
		int32 CurrentClip;
	//weapon reloaded ammo
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
		int32 CurrentAmmo;

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
	void StopReloading();
	void TurnOnTorch();

	void TurnOffTorch();
protected:

	virtual void SimulateWeaponFire();

	virtual void StopSimulatingWeaponFire();

	FVector GetMuzzleLocation() const;

	FVector GetMuzzleDirection() const;

	UAudioComponent* PlayWeaponSound(USoundCue* SoundToPlay);

	float PlayWeaponAnimation(UAnimMontage* Animation, float InPlayRate = 1.f, FName StartSectionName = NAME_None);

	void StopWeaponAnimation(UAnimMontage* Animation);

private:


	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
		USoundCue* FireSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
		USoundCue* EquipSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
		USoundCue* ReloadSound;



	UPROPERTY(EditDefaultsOnly)
		UParticleSystem* MuzzleFX;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		UAnimMontage* ReloadAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		UAnimMontage* EquipAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		UAnimMontage* FireAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		float ReloadAnimDuration;

	/* Time to assign on equip when no animation is found */
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		float EquipAnimDuration;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		float FireAnimDuration;

	UPROPERTY(EditDefaultsOnly)
		FName MuzzleAttachPoint;

	UParticleSystemComponent* MuzzlePSC;
	/************************************************************************/
	/* Visual Handlers                                                      */
	/************************************************************************/

	void VisualInstantHit(const FVector& ImpactPoint);

	void VisualImpactEffects(const FHitResult& Impact);

	void VisualTrailEffects(const FVector& EndPoint);



	/* Particle FX played when a surface is hit. */
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ABaseImpactEffect> ImpactTemplate;

	UPROPERTY(EditDefaultsOnly)
		FName TrailTargetParam;

	UPROPERTY(EditDefaultsOnly)
		UParticleSystem* TrailFX;

	UPROPERTY(EditDefaultsOnly)
		UParticleSystem* TracerFX;

	/* Minimum firing distance before spawning tracers or trails. */
	UPROPERTY(EditDefaultsOnly)
		float MinimumProjectileSpawnDistance;

	UPROPERTY(EditDefaultsOnly)
		int32 TracerRoundInterval;

	/* Keeps track of number of shots fired */
	int32 BulletsShotCount;

	bool bPlayingFireAnim;

protected:
	FHitResult WeaponTrace(const FVector &TraceFrom, const FVector &TraceTo) const;

	void ProcessInstantHit(const FHitResult &Impact, const FVector &Origin, const FVector &ShootDir, int32 RandomSeed, float ReticleSpread);

	FVector GetAdjustedAim() const;
	FVector GetCameraDamageStartLocation(const FVector& AimDir) const;



};
