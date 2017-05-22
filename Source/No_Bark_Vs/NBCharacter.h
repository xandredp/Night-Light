
#pragma once

#include "Perception/PawnSensingComponent.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
#include "BaseWeapon.h"
#include "TypeClass.h"
#include "NBCharacter.generated.h"


//GunSkeltal -> bullet spawn point  = MuzzleTip



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

	/*The function that is going to play the sound and report it to our game*/
	UFUNCTION(BlueprintCallable, Category = "AI")
		void ReportNoise(USoundBase* SoundToPlay, float Volume);

	/*A Pawn Noise Emitter component which is used in order to emit the sounds to nearby AIs*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UPawnNoiseEmitterComponent* PawnNoiseEmitterComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void EquipPrimaryWeapon();

	void GetEquipment(int index);

	void SpawnWeapon(TSubclassOf <class ABaseWeapon> iWeaponClass);

	UFUNCTION(BlueprintImplementableEvent)
		void AttachEquipmentToHand();

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawn)
		TSubclassOf <class ABaseWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
	ABaseWeapon *CurrentWeapon;

	/* Return socket name for attachments (to match the socket in the character skeleton) */
	UFUNCTION(BlueprintCallable, Category = "Equip")
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

	/************************************************************************/
	/* Pickups                                                              */
	/************************************************************************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		float MaxInteractDistance;

private:

	/* Attachpoint for active weapon/item in hands */
	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
		FName WeaponAttachPoint;

	/* Attachpoint for items carried on the belt/pelvis. */
	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
		FName MeleeAttachPoint;

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

	void FireWeapon();

	/************************************************************************/
	/* Pickups                                                              */
	/************************************************************************/

	UFUNCTION(BlueprintCallable, Category = "PickupCondition")
		void CheckForInteractables();

	UFUNCTION(BlueprintCallable, Category = "PickupCondition")
		ABaseInteractable* GetInteractableInView();



public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

