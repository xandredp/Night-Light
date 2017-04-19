
#pragma once


#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
#include "BaseWeapon.h"
#include "NBCharacter.generated.h"

//GunSkeltal -> bullet spawn point  = MuzzleTip

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
		FText Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Action;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Value;
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

UCLASS(config = Game)
class NO_BARK_VS_API ANBCharacter : public ACharacter
{
	GENERATED_BODY()
	
		/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	// Sets default values for this character's properties
	ANBCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	//UPROPERTY(Transient, Replicated)
		bool bWantsToRun;

	/** Sprinting rate this decides how fast sprint will be. */
	UPROPERTY(EditDefaultsOnly, Category = Movement)
		float SpeedValue;
	/************************************************************************/
	/* weapon use                                                           */
	/************************************************************************/
	
	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(EditDefaultsOnly, Category = Spawn)
		TSubclassOf <class ABaseWeapon> WeaponClass;

	ABaseWeapon *CurrentWeapon;



protected:

	/************************************************************************/
	/* Movement                                                        */
	/************************************************************************/
	void OnResetVR();

	void MoveForward(float Value);

	void MoveRight(float Value);

	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	void OnStartSprinting();

	void OnStopSprinting();

	void SetSprinting(bool NewSprinting);

	void OnCrouchToggle();

	/************************************************************************/
	/* Pickups                                                              */
	/************************************************************************/

	UFUNCTION(BlueprintCallable, Category = "PickupCondition")
		void CheckForInteractables();

	/************************************************************************/
	/* Health                                                               */
	/************************************************************************/
	//UFUNCTION(BlueprintCallable, Category = "PlayerCondition")
	//	float GetMaxHealth() const;

	//UFUNCTION(BlueprintCallable, Category = "PlayerCondition")
	//	float GetHealth() const;

	//UFUNCTION(BlueprintCallable, Category = "PlayerCondition")
	//	bool IsAlive() const;

	//UFUNCTION(BlueprintCallable, Category = "Movement")
	//	virtual bool IsSprinting() const;

	///* Client/local call to update sprint state  */
	//virtual void SetSprinting(bool NewSprinting);

	//float GetSprintingSpeedModifier() const;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

