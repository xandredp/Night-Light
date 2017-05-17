#pragma once

#include "Engine/DataTable.h"
#include "TypeClass.generated.h"

UENUM()
enum class EBotBehaviorType : uint8
{
	// passive patrolling mode
	Neutral,
	//hearing or seeing someone. 
	Suspicious,
	//Running towards the player and attacking
	Agression,
	// Torches on he is in the light and not moving
	Stunned,
	//Move out of the light.
	Flee, 
};


UENUM()
enum class EInventorySlot : uint8
{
	/* For currently equipped items/weapons */
	Hands,

	/* For primary weapons on spine bone */
	Primary,

	/* For secondary weapons on spine bone */
	Secondary,

	/* For Melee weapons on bone */
	Melee,
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None,

	Weapon,

	MeleeWeapon,

	Armor,

	Food,

	Potion,

	Grenades,


};
// Struct of things I can Craft. 
USTRUCT(BlueprintType)
struct FCraftingInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ComponentID; //item used to create Item (  cog)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ProductID; // item that gets created (barrier)

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bDestroyItemA; // destroy component
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bDestroyItemB; // destroy product id
};


//TableRowBase for Data TABLE. 
//This will check if Items are allowed.
//database system. only default functions will be written here
USTRUCT(BlueprintType)
struct FInventoryItem : public FTableRowBase
{
	GENERATED_BODY()

public:

	FInventoryItem()
	{
		Name = FText::FromString("Item");
		Action = FText::FromString("Use");
		Description = FText::FromString("Description here");
		Value = 10;
		MaxStackNumber = 1;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class APickup>ItemPickup;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ABaseWeapon>ItemWeaponClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Action;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Value;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EItemType eItemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* Thumbnail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 MaxStackNumber;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FCraftingInfo> CraftCombinations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bCanBeUsed;

	//ureal requires oveloading the data. 
	bool operator == (const FInventoryItem& Item) const
	{
		if (ItemID == Item.ItemID)
			return true;
		else return false;

	}
};


//These are the  inventory Items // changeable 
USTRUCT(BlueprintType)
struct FCurrentInventoryItemInfo 
{
	GENERATED_BODY()

public:

	FCurrentInventoryItemInfo()
	{
		CurrentStackNumber = 0;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 CurrentStackNumber;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ItemIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FInventoryItem ItemInfo;

	//ureal requires oveloading the data. 
	bool operator == (const FCurrentInventoryItemInfo& CurrentItem) const
	{
		if (ItemInfo.ItemID == CurrentItem.ItemInfo.ItemID)
			return true;
		else return false;

	}
};


USTRUCT()
struct FTakeHitInfo
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY()
		float ActualDamage;

	UPROPERTY()
		UClass* DamageTypeClass;

	UPROPERTY()
		TSubclassOf<class ANBCharacter> PawnInstigator;

	UPROPERTY()
		TSubclassOf<class AActor> DamageCauser;

	UPROPERTY()
		uint8 DamageEventClassID;

	UPROPERTY()
		bool bKilled;

private:

	UPROPERTY()
		uint8 EnsureReplicationByte;

	UPROPERTY()
		FDamageEvent GeneralDamageEvent;

	UPROPERTY()
		FPointDamageEvent PointDamageEvent;

	UPROPERTY()
		FRadialDamageEvent RadialDamageEvent;

public:
	FTakeHitInfo()
		: ActualDamage(0),
		DamageTypeClass(nullptr),
		PawnInstigator(nullptr),
		DamageCauser(nullptr),
		DamageEventClassID(0),
		bKilled(false),
		EnsureReplicationByte(0)
	{}

	FDamageEvent& GetDamageEvent()
	{
		switch (DamageEventClassID)
		{
		case FPointDamageEvent::ClassID:
			if (PointDamageEvent.DamageTypeClass == nullptr)
			{
				PointDamageEvent.DamageTypeClass = DamageTypeClass ? DamageTypeClass : UDamageType::StaticClass();
			}
			return PointDamageEvent;

		case FRadialDamageEvent::ClassID:
			if (RadialDamageEvent.DamageTypeClass == nullptr)
			{
				RadialDamageEvent.DamageTypeClass = DamageTypeClass ? DamageTypeClass : UDamageType::StaticClass();
			}
			return RadialDamageEvent;

		default:
			if (GeneralDamageEvent.DamageTypeClass == nullptr)
			{
				GeneralDamageEvent.DamageTypeClass = DamageTypeClass ? DamageTypeClass : UDamageType::StaticClass();
			}
			return GeneralDamageEvent;
		}
	}


	void SetDamageEvent(const FDamageEvent& DamageEvent)
	{
		DamageEventClassID = DamageEvent.GetTypeID();
		switch (DamageEventClassID)
		{
		case FPointDamageEvent::ClassID:
			PointDamageEvent = *((FPointDamageEvent const*)(&DamageEvent));
			break;
		case FRadialDamageEvent::ClassID:
			RadialDamageEvent = *((FRadialDamageEvent const*)(&DamageEvent));
			break;
		default:
			GeneralDamageEvent = DamageEvent;
		}
	}


	void EnsureReplication()
	{
		EnsureReplicationByte++;
	}
};