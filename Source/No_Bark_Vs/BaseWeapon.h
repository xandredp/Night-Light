// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"
#include "Pickup.h"
#include "BaseWeapon.generated.h"

/**
 * 
 */
//
//UENUM(BlueprintType)
//namespace EWeaponProjectile
//{
//	enum ProjectileType
//	{
//		EBullet			UMETA(DisplayName = "Bullet"),
//		ESpread			UMETA(DisplayName = "Spread"),
//		EProjectile	UMETA(DisplayName = "Projectile"),
//
//	};
//}
//
//TableRowBase for Data TABLE. 
USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase
{
	GENERATED_BODY()

public:


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 MaxAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float WeaponRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TimeBetweenShots;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float WeaponSpread;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bCanBeUsed;


};


UCLASS()
class NO_BARK_VS_API ABaseWeapon : public APickup
{
	GENERATED_BODY()

public:
	//UFUNCTION(BlueprintCallable, Category = "Config")
	//void Fire();
	//
	//UFUNCTION(BlueprintCallable, Category = "Config")
	//void Instant_Fire();

	//UPROPERTY(EditDefaultsOnly, Category = "Config")
	//	FWeaponData WeaponConfig;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	//	TSubobjectPtr<UBoxComponent> CollisionComp;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	//	TSubobjectPtr<USkeletalMeshComponent> WeaponMesh;
	
};
