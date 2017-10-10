
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
//#include "Animation Instances/CharacterAnimInstance.h"
#include "AI/Spawning/MonsterSpawner.h"
#include "PlayerSensingPawn.h"
//#include "Perception/PawnSensingComponent.h"
#include "Engine/DataTable.h"
#include "Core/BaseWeapon.h"
#include "Core/BaseTorch.h"
#include "Core/TypeClass.h"
#include "NBCharacter.generated.h"


//GunSkeltal -> bullet spawn point  = MuzzleTip

class UInputComponent;

UCLASS(config = Game)
class ANBCharacter : public ACharacter
{
	GENERATED_BODY()
		
public:
	//	/** Camera boom positioning the camera behind the character */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collision")
		class USkeletalMeshComponent* CharacterMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collision")
		class USkeletalMeshComponent* CameraMovemetMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collision")
		class USkeletalMeshComponent* FPSCharacterArmMesh;

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

	//gain Health
	FTimerHandle StartHealTimerHandle;

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

	UFUNCTION()
		 void OnSeeEnemy(APawn* Pawn);

	void EquipPrimaryWeapon();

	/************************************************************************/
	/* Torch*/
	/************************************************************************/
	UFUNCTION(BlueprintCallable, Category = "Torch")
	void TurnOnTorch();

	UFUNCTION(BlueprintCallable, Category = "Torch")
	void TurnOffTorch();

	void TorchCrank();

	void PowerUpTorch();


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
	/* Anim use                                                           */
	/************************************************************************/
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	//	UCharacterAnimInstance* AnimInstance;
	/* Retrieve Pitch/Yaw from current camera */
	UFUNCTION(BlueprintCallable, Category = "Animation")
		FRotator GetAimOffsets() const;

	UFUNCTION(BlueprintCallable, Category = "Animation")
		bool GetIsCrouched() const;

	UFUNCTION(BlueprintCallable, Category = "Pickup")
		void PlayPickUpAnimation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		UAnimInstance* ArmAnimInstance;

	UFUNCTION(BlueprintCallable, Category = "Animation")
		float PlayArmAnimation(UAnimMontage * Animation, float InPlayRate, FName StartSectionName);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
		class UAnimMontage* ReloadAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
		class UAnimMontage* EquipAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
		class UAnimMontage* FireAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
		class UAnimMontage* DrawingAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
		class UAnimMontage* PickingUpAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
		class UAnimMontage* NoClipAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
		class UAnimMontage* StartCrankAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
		class UAnimMontage* CrankingAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
		class UAnimMontage* EndCrankAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		bool IsAnimPlaying;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		bool IsTorchCrancking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		bool IsTorchCrankerUp;

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

	/************************************************************************/
	/* Torch use                                                           */
	/************************************************************************/

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawn)
		TSubclassOf <class ABaseTorch> TorchClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		ABaseTorch *CurrentTorch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		APlayerSensingPawn *SensingComponentPawn;

	UFUNCTION(BlueprintCallable, Category = "Torch")
		void SpawnTorch();

	UFUNCTION(BlueprintImplementableEvent)
		void AttachTorchToGun();

public:
	/* Return socket name for attachments (to match the socket in the character skeleton) */
	UFUNCTION(BlueprintCallable, Category = "Equip")
	FName GetInventoryAttachPoint(EInventorySlot Slot) const;


	/************************************************************************/
	/* Status																*/
	/************************************************************************/

	/**LockMovement  **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		bool LockMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		float CurrentStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		float HealthRegenRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		float StaminaRegenRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		float SprintDeductionRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		float StaminaTimerRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		float HealthTimerRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		float walkingSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		float MaxSprintSpeed;

	/* Attachpoint for secondary weapons */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		bool bIsDead;

	/* scorecounter*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		int CurrentScore;

	/* DecreaseStamina, used by timer. */
	UFUNCTION(BlueprintCallable, Category = "Condition")
	void DecreaseHealth(float decreaseVal);

	UFUNCTION(BlueprintCallable, Category = "Condition")
	void IncreaseHealth(float increaseVal);

	UFUNCTION(BlueprintCallable, Category = "Condition")
		void IncreaseHealthByTime();

	UFUNCTION(BlueprintCallable, Category = "Condition")
	void DecreaseStamina();

	UFUNCTION(BlueprintCallable, Category = "Condition")
	void IncreaseStamina();

	UFUNCTION(BlueprintCallable, Category = "Condition")
	void IncreaseScore(int increaseVal);

	UFUNCTION(BlueprintCallable, Category = "Condition")
	void DecreaseScore(int decreaseVal);

	/*This value is used in Enemy to detect the player 1.0 will take longer and 0.0 will be immediate detection formular
	PawnSeenValue * DetectionMaxTime*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition")
		float ValToMakePawnUnDetected;

	UFUNCTION(BlueprintImplementableEvent)
		void DestroyAndBackToMenu();

	//HitBlur******************************************************/
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		float HitBlur;

	UPROPERTY(EditDefaultsOnly, Category = Effects)
		TSubclassOf<UCameraShake> HitCameraShake;

	/************************************************************************/
	/* Pickups                                                              */
	/************************************************************************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		float MaxInteractDistance;

private:
	/* Attachpoint for active weapon/item in hands */
	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
		FName TorchAttachPoint;


	/* Attachpoint for active weapon/item in hands */
	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
		FName CameraAttachPoint;

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
	void MoveForward(float Value);

	void MoveRight(float Value);

	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	void OnStartSprinting();

	void OnStopSprinting();

	void OnStartHealing();

	void OnStopHealing();

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
//	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

