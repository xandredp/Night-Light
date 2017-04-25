
#pragma once


#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
#include "BaseWeapon.h"
#include "NBCharacter.generated.h"

//GunSkeltal -> bullet spawn point  = MuzzleTip


UENUM()
enum class EInventorySlot : uint8
{
	/* For currently equipped items/weapons */
	Hands,

	/* For primary weapons on spine bone */
	Primary,

	/* For secondary weapons on spine bone */
	Secondary,

	/* For Knife weapons on bone */
	Knife,
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

	UCharacterMovementComponent* MoveComp;

	FTimerHandle StartSprintingTimerHandle;
	FTimerHandle StopSprintingTimerHandle;

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

	/************************************************************************/
	/* weapon use                                                           */
	/************************************************************************/
	
	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(EditDefaultsOnly, Category = Spawn)
		TSubclassOf <class ABaseWeapon> WeaponClass;

	ABaseWeapon *CurrentWeapon;

	/* Return socket name for attachments (to match the socket in the character skeleton) */
	FName GetInventoryAttachPoint(EInventorySlot Slot) const;


	/************************************************************************/
	/* Status																*/
	/************************************************************************/

	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		float CurrentStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		float CurrentMagic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		float MaxMagic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		float StaminaRegenRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		float SprintDeductionRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		float StaminaTimerRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		float HealthTimerRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		float MagicTimerRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		float walkingSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		float MaxSprintSpeed;

	/* DecreaseStamina, used by timer. */
	void DecreaseHealth(float decreaseVal);
	void IncreaseHealth(float increaseVal);
	void DecreaseStamina();
	void IncreaseStamina();


private:

	/* Attachpoint for active weapon/item in hands */
	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
		FName WeaponAttachPoint;

	/* Attachpoint for items carried on the belt/pelvis. */
	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
		FName KnifeAttachPoint;

	/* Attachpoint for primary weapons */
	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
		FName PrimaryAttachPoint;

	/* Attachpoint for secondary weapons */
	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
		FName SecondaryAttachPoint;



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

