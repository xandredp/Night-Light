
#pragma once

#include "Animation Instances/CharacterAnimInstance.h"
#include "AI/Spawning/MonsterSpawner.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
#include "Core/BaseWeapon.h"
#include "Core/TypeClass.h"
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

	//reduce stamina
	FTimerHandle StartSprintingTimerHandle;
	//gain stamina
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

	// The class for handling spawning
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		AMonsterSpawner* Spawner;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void EquipPrimaryWeapon();
	void EquipSecondaryWeapon();
	void EquipMeleeWeapon();
	void EquipOthers(int i_SlotNumber);

	// Spawn monster function called by input binding
	void Spawn();

	void GetEquipment(int index);

	void SpawnWeapon(TSubclassOf <class ABaseWeapon> iWeaponClass);
	void SpawnWeaponOnSlot(TSubclassOf <class ABaseWeapon> iWeaponClass, EInventorySlot Slot);

	UFUNCTION(BlueprintImplementableEvent)
		void AttachEquipmentToHand();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;


	/************************************************************************/
	/* Anim use                                                           */
	/************************************************************************/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		UCharacterAnimInstance* AnimInstance;
	/* Retrieve Pitch/Yaw from current camera */
	UFUNCTION(BlueprintCallable, Category = "Animation")
		FRotator GetAimOffsets() const;

	UFUNCTION(BlueprintCallable, Category = "Animation")
		bool GetIsCrouched() const;

	/************************************************************************/
	/* weapon use                                                           */
	/************************************************************************/
	
	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawn)
		TSubclassOf <class ABaseWeapon> WeaponClass;
	//Hand Held Item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
	ABaseWeapon *CurrentWeapon;
	//Primary Weapon Held Item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		ABaseWeapon *PrimaryWeapon;
	//Secondary Weapon Held Item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		ABaseWeapon *SecondaryWeapon;
	//Melee  Weapon Held Item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		ABaseWeapon *MeleeWeapon;
	//Pistol 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		ABaseWeapon *PistolWeapon;

public:
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
	/* Attachpoint for secondary weapons */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		bool bIsDead;

	/* DecreaseStamina, used by timer. */
	UFUNCTION(BlueprintCallable, Category = "Condition")
	void DecreaseHealth(float decreaseVal);
	void IncreaseHealth(float increaseVal);
	void DecreaseStamina();
	void IncreaseStamina();


	UFUNCTION(BlueprintImplementableEvent)
		void DestroyAndBackToMenu();

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

	/* Attachpoint for secondary weapons */
	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
		bool bIsFiring;



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
	void StopFireWeapon();
	void ReloadWeapon();

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
