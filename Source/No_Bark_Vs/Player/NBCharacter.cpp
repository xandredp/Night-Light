// Fill out your copyright notice in the Description page of Project Settings.
#include "Player/NBCharacter.h"
#include "Core/No_Bark_Vs.h"
#include "Core/BaseInteractable.h"
#include "Player/PlayController.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
//#include "Perception/PawnSensingComponent.h"
//#include "GameFramework/InputSettings.h"
//#include "HeadMountedDisplayFunctionLibrary.h"
//#include "Engine.h"



ANBCharacter::ANBCharacter()
{
	TorchAttachPoint = TEXT("Light_Socket");
	CameraAttachPoint = TEXT("Head_Socket");
	WeaponAttachPoint = TEXT("Weapon_Socket");
	MeleeAttachPoint = TEXT("Thigh_Socket");
	PrimaryAttachPoint = TEXT("Spine_Socket");
	SecondaryAttachPoint = TEXT("Clavicle_Socket");

	//Status
	MaxInteractDistance = 500.0f;
	walkingSpeed = 250.0f;
	MaxSprintSpeed = 400.0f;
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
	CurrentStamina = 100.0f;
	MaxStamina = 100.0f;
	HealthRegenRate = 1.0f;
	StaminaRegenRate = 1.0f;
	SprintDeductionRate = 1.3f;
	StaminaTimerRate = 0.5f;
	HealthTimerRate = 1.0f;
	bIsFiring = false;
	IsTorchCrankerUp = false;
	HitBlur = 0;
	ValToMakePawnUnDetected = 1.0;
	// Item

	PawnNoiseEmitterComp = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("PawnNoiseEmitterComp"));


	MoveComp = GetCharacterMovement();
	// Adjust jump to make it less floaty
	MoveComp->GravityScale = 1.5f;
	MoveComp->JumpZVelocity = 620;
	MoveComp->bCanWalkOffLedgesWhenCrouching = true;
	MoveComp->MaxWalkSpeedCrouched = 200;
	MoveComp->MaxWalkSpeed = walkingSpeed;

	// Enable movements
	MoveComp->GetNavAgentPropertiesRef().bCanCrouch = true;
	MoveComp->GetNavAgentPropertiesRef().bCanFly = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	CharacterMesh = GetMesh();// CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	////CharacterMesh = GetMesh();// CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerMesh"));
	//CharacterMesh->SetupAttachment(RootComponent);
			
	
	// Create a follow camera
	CameraMovemetMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPSArmMeshForCameraMovemet"));
	CameraMovemetMesh->SetOnlyOwnerSee(true);
	CameraMovemetMesh->SetupAttachment(GetCapsuleComponent());
	CameraMovemetMesh->bCastDynamicShadow = false;
	CameraMovemetMesh->CastShadow = false;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	FPSCharacterArmMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPSArmMesh"));
	FPSCharacterArmMesh->SetOnlyOwnerSee(true);
	FPSCharacterArmMesh->SetupAttachment(FollowCamera);
	FPSCharacterArmMesh->bCastDynamicShadow = false;
	FPSCharacterArmMesh->CastShadow = false;
	FPSCharacterArmMesh->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	FPSCharacterArmMesh->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);
	LockMovement = false;
	IsBeingAttacked = false;
}

void ANBCharacter::BeginPlay()
{
	Super::BeginPlay();
//	FollowCamera->AttachToComponent(CharacterMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), CameraAttachPoint);

	//FollowCamera->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, CameraAttachPoint);
	//AnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	bIsDead = false;
	CharacterMesh->SetHiddenInGame(true, true);

	if (WeaponClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			
		}
	}


}
		
void ANBCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	CheckForInteractables(); 
	if (HitBlur > 0)
	{
		HitBlur--;
	}
}

void ANBCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	if (LockMovement == false)
	{
		PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
		PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

		PlayerInputComponent->BindAction("SprintHold", IE_Pressed, this, &ANBCharacter::OnStartSprinting);
		PlayerInputComponent->BindAction("SprintHold", IE_Released, this, &ANBCharacter::OnStopSprinting);

		PlayerInputComponent->BindAction("CrouchToggle", IE_Released, this, &ANBCharacter::OnCrouchToggle);
		PlayerInputComponent->BindAction("PrimaryWeapon", IE_Pressed, this, &ANBCharacter::EquipPrimaryWeapon);

		PlayerInputComponent->BindAxis("MoveForward", this, &ANBCharacter::MoveForward);
		PlayerInputComponent->BindAxis("MoveRight", this, &ANBCharacter::MoveRight);

		PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
		PlayerInputComponent->BindAxis("TurnRate", this, &ANBCharacter::TurnAtRate);
		PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
		PlayerInputComponent->BindAxis("LookUpRate", this, &ANBCharacter::LookUpAtRate);
	}
	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ANBCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ANBCharacter::TouchStopped);

	// Flashlight Torch Controls
	PlayerInputComponent->BindAction("TorchOn", IE_Pressed, this, &ANBCharacter::TurnOnTorch);
	PlayerInputComponent->BindAction("TorchOff", IE_Pressed, this, &ANBCharacter::TurnOffTorch);

	PlayerInputComponent->BindAction("TorchCrank", IE_Pressed, this, &ANBCharacter::TorchCrank);

	PlayerInputComponent->BindAction("TorchPower", IE_Pressed, this, &ANBCharacter::PowerUpTorch);

	// Weapons

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ANBCharacter::FireWeapon);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ANBCharacter::StopFireWeapon);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ANBCharacter::ReloadWeapon);
}

void ANBCharacter::OnSeeEnemy(APawn * Pawn)
{
	if (CurrentTorch != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "You Seean an Enemy!");
	}

}

void ANBCharacter::EquipPrimaryWeapon()
{
	
	if (WeaponClass == NULL)
	{			
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "WeaponClassIsEmpty");
	}
	else
	{
		SpawnWeapon(WeaponClass);
		AttachEquipmentToHand();
	}
}

void ANBCharacter::SpawnTorch()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;
	// if current weapon is empty assign current weapon
	if (CurrentTorch == nullptr)
	{
		CurrentTorch = GetWorld()->SpawnActor<ABaseTorch>(TorchClass, SpawnParams);
	} 
	CurrentTorch->SetOwningPawn(this);

	//Attach Sensing component This is used as Torch collision component
	if (SensingComponentPawn)
	{
		SensingComponentPawn->AttachToComponent(FPSCharacterArmMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, "Sense_Socket");
	}

	AttachTorchToGun();
}

void ANBCharacter::SpawnWeapon(TSubclassOf<class ABaseWeapon> iWeaponClass)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;
	// if current weapon is empty assign current weapon
	if (CurrentWeapon == nullptr)
	{
		CurrentWeapon = GetWorld()->SpawnActor<ABaseWeapon>(iWeaponClass, SpawnParams);

		if (DrawingAnimation != NULL)
		{
			ArmAnimInstance = FPSCharacterArmMesh->GetAnimInstance();
			if (ArmAnimInstance != NULL)
			{
				ArmAnimInstance->Montage_Play(DrawingAnimation, 1.0f);
			}
		}
	}
	else
	{
		CurrentWeapon->Destroy();
		CurrentWeapon = GetWorld()->SpawnActor<ABaseWeapon>(iWeaponClass, SpawnParams);
	}
	CurrentWeapon->SetOwningPawn(this);
}

	
void ANBCharacter::DecreaseHealth(float decreaseVal)
{
	if (CurrentHealth > 0)
	{
		CurrentHealth -= decreaseVal;	
		HitBlur = 10;

		APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
		PC->ClientPlayCameraShake(HitCameraShake, 1, ECameraAnimPlaySpace::CameraLocal, FRotator(0, 0, 0));

	}
	else
	{
		bIsDead = true;
		DestroyAndBackToMenu();
	}
	/*Health Regenration Diabled to enable comment this one back*/
	//GetWorldTimerManager().SetTimer(StartHealTimerHandle, this, &ANBCharacter::IncreaseHealthByTime, HealthTimerRate, true);

}

void ANBCharacter::IncreaseHealth(float increaseVal)
{
	CurrentHealth += increaseVal;
	
}
void ANBCharacter::IncreaseHealthByTime()
{
	if (CurrentHealth >= MaxHealth)
	{
		//Stops Health Increase
		//GetWorldTimerManager().ClearTimer(StartHealTimerHandle);
	}
	else
	{
		CurrentHealth = CurrentHealth + HealthRegenRate;
	
	}
}
void ANBCharacter::DecreaseStamina()
{
	if (CurrentHealth <= 0.0f)
	{
		OnStopSprinting();
	}
	else
	{
		CurrentStamina = CurrentStamina - SprintDeductionRate;
	}
}

void ANBCharacter::IncreaseStamina()
{
	if (CurrentStamina >= MaxStamina)
	{
		GetWorldTimerManager().ClearTimer(StopSprintingTimerHandle);
	}
	else
	{
		CurrentStamina = CurrentStamina + StaminaRegenRate;
	}
}

void ANBCharacter::IncreaseScore(int increaseVal)
{
	CurrentScore += increaseVal;
	APlayController* playerController = Cast<APlayController>(GetController());
	playerController->MyCurrentCurrency = CurrentScore;
}

void ANBCharacter::DecreaseScore(int decreaseVal)
{
	CurrentScore -= decreaseVal;
	APlayController* playerController = Cast<APlayController>(GetController());
	playerController->MyCurrentCurrency = CurrentScore;
}

void ANBCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void ANBCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void ANBCharacter::CheckForInteractables()
{
	
	APlayController* playController = Cast<APlayController>(GetController());
	if (ABaseInteractable* interactable = GetInteractableInView())
	{
		playController->CurrentInteractable = interactable;
		return;
	}
	// If we didn't hit anything, or thing we hit was on to a interactable set current interactable nullptr. 
	else
	{
		playController->CurrentInteractable = nullptr;
	}
}

ABaseInteractable* ANBCharacter::GetInteractableInView()
{
	FVector CamLoc;
	FRotator CamRot;

	if (Controller == nullptr)
		return nullptr;

	Controller->GetPlayerViewPoint(CamLoc, CamRot);
	const FVector TraceStart = CamLoc;
	const FVector Direction = CamRot.Vector();
	const FVector TraceEnd = TraceStart + (Direction * MaxInteractDistance);

	FCollisionQueryParams TraceParams(TEXT("TraceUsableActor"), true, this);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = false;

	/* Not tracing complex uses the rough collision instead making tiny objects easier to select. */
	TraceParams.bTraceComplex = false;

	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, TraceParams);
	//GetWorld()->LineTraceSingleByObjectType(Hit, TraceStart, TraceEnd, ECC_Visibility, TraceParams);

	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Blue, false, 1.0f);

	return Cast<ABaseInteractable>(Hit.GetActor());
}

void ANBCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ANBCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ANBCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);

		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, this->GetName() + TEXT(" - Making a virtual MoveForward noise!"));
		MakeNoise(100, this, GetActorLocation());
	}
}

void ANBCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, this->GetName() + TEXT(" - Making a virtual MoveRight noise!"));
		MakeNoise(100, this, GetActorLocation());
	}
}
//When sprinting the health will not gain and Stamina will decrease.  the effect will stop when firing
void ANBCharacter::OnStartSprinting()
{
	if (bIsFiring == false)
	{
		MoveComp->MaxWalkSpeed = MaxSprintSpeed;
		if (Role == ROLE_Authority)
		{
			//Stamina Decrease
			GetWorldTimerManager().SetTimer(StartSprintingTimerHandle, this, &ANBCharacter::DecreaseStamina, StaminaTimerRate, true);
			//Stops Stamina increase
			GetWorldTimerManager().ClearTimer(StopSprintingTimerHandle);
			//Stops Health Increase
			GetWorldTimerManager().ClearTimer(StartHealTimerHandle);
			// Sprinting is noisy
			MakeNoise(100, this, GetActorLocation());

		}
	}
}
// When not sprinting stamina will gain and health will also gain. 
void ANBCharacter::OnStopSprinting()
{
	if (bIsFiring == false)
	{
		MoveComp->MaxWalkSpeed = walkingSpeed;

		if (Role == ROLE_Authority)
		{
			//stamina gain
			GetWorldTimerManager().SetTimer(StopSprintingTimerHandle, this, &ANBCharacter::IncreaseStamina, StaminaTimerRate, true);
			//stops decreasing stamina
			GetWorldTimerManager().ClearTimer(StartSprintingTimerHandle);
			//health gain
			GetWorldTimerManager().SetTimer(StartHealTimerHandle, this, &ANBCharacter::IncreaseHealthByTime, HealthTimerRate, true);
		}
	}

}

void ANBCharacter::OnCrouchToggle()
{
	if (bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}

}

float ANBCharacter::PlayArmAnimation(UAnimMontage * Animation, float InPlayRate, FName StartSectionName)
{
	float Duration = 0.0f;

	if (Animation)
	{
		Duration = PlayAnimMontage(Animation, InPlayRate, StartSectionName);
	}
	
	return Duration;
}

void ANBCharacter::FireWeapon()
{

	if (bIsFiring == false)
	{
		bIsFiring = true;

		if (CurrentWeapon)
		{
			
			int32 CurrentAmmoInWeapon = CurrentWeapon->CurrentAmmo;
			int32 CurrentClipInWeapon = CurrentWeapon->CurrentClip;
			if (CurrentAmmoInWeapon == 0)
			{
				if (CurrentClipInWeapon > 0)
				{
					ReloadWeapon();
				}
				else
				{
					if (NoClipAnimation != NULL)
					{
						if (IsAnimPlaying != true)
						{
							ArmAnimInstance = FPSCharacterArmMesh->GetAnimInstance();
							if (ArmAnimInstance != NULL)
							{
								ArmAnimInstance->Montage_Play(NoClipAnimation, 1.0f);

							}
						}					
					}
				}
			}
			else if (CurrentAmmoInWeapon > 0)
			{		
				//Trigger Fire animation only if there is ammo.
				//this is where fire animation is triggered
				if (IsAnimPlaying != true)
				{
					if (FireAnimation != NULL)
					{
						ArmAnimInstance = FPSCharacterArmMesh->GetAnimInstance();
						if (ArmAnimInstance != NULL)
						{
							ArmAnimInstance->Montage_SetPosition(FireAnimation, 5.0f);
							ArmAnimInstance->Montage_Play(FireAnimation, 1.0f);	
							CurrentWeapon->SetTimerForFiring();
						}
					}
				}
			}
		}
	}
}

void ANBCharacter::StopFireWeapon()
{
	bIsFiring = false;
	if (CurrentWeapon)
	{
		CurrentWeapon->StopTimerForFiring();
	}
	
}

void ANBCharacter::ReloadWeapon()
{
	if (CurrentWeapon)
	{
		if (CurrentWeapon->CurrentClip > 0)
		{
			if (IsAnimPlaying == false)
			{
				if (ReloadAnimation != NULL)
				{
					ArmAnimInstance = FPSCharacterArmMesh->GetAnimInstance();
					if (ArmAnimInstance != NULL)
					{
						ArmAnimInstance->Montage_Play(ReloadAnimation, 1.0f);
					}
				}
				CurrentWeapon->ReloadAmmo();
			}
		}
	}
}

FName ANBCharacter::GetInventoryAttachPoint(EInventorySlot Slot) const
{
	/* Return the socket name for the specified storage slot */
	switch (Slot)
	{
	case EInventorySlot::Hands:
		return WeaponAttachPoint;
	case EInventorySlot::Primary:
		return PrimaryAttachPoint;
	case EInventorySlot::Secondary:
		return SecondaryAttachPoint;
	case EInventorySlot::Melee:
		return MeleeAttachPoint;
	default:
		// Not implemented.
		return "";
	}
}

void ANBCharacter::ReportNoise(USoundBase* SoundToPlay, float Volume)
{
	//If we have a valid sound to play, play the sound and
	//report it to our game
	if (SoundToPlay)
	{
		//Play the actual sound
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundToPlay, GetActorLocation(), Volume);

		//Report that we've played a sound with a certain volume in a specific location
		MakeNoise(Volume, this, GetActorLocation());
	}

}

FRotator ANBCharacter::GetAimOffsets() const
{
	const FVector AimDirWS = GetBaseAimRotation().Vector();
	const FVector AimDirLS = ActorToWorld().InverseTransformVectorNoScale(AimDirWS);
	const FRotator AimRotLS = AimDirLS.Rotation();

	return AimRotLS;
}

bool ANBCharacter::GetIsCrouched() const
{
	return bIsCrouched;
}


void ANBCharacter::TurnOnTorch()
{
	if (CurrentTorch)
	{
		CurrentTorch->TorchOnOff(true);
	}
}


void ANBCharacter::TurnOffTorch()
{
	if (CurrentTorch)
	{
		CurrentTorch->TorchOnOff(false);
	}
}
void ANBCharacter::PlayPickUpAnimation()
{
	if (IsAnimPlaying == false)
	{
		if (PickingUpAnimation != NULL)
		{
			ArmAnimInstance = FPSCharacterArmMesh->GetAnimInstance();
			if (ArmAnimInstance != NULL)
			{
				ArmAnimInstance->Montage_Play(PickingUpAnimation, 1.0f);
			}
		}
	}

}

void ANBCharacter::TorchCrank()
{
	APlayController* playerController = Cast<APlayController>(GetController());
	
	//To play cranking there are 3 animations start - cranking - end
	//If no anim montage is playing
	
	if (CurrentWeapon)
	{
		if (CurrentTorch!=nullptr)
		{

			if (IsTorchCrankerUp == true)
			{
				if (CrankingAnimation != NULL)
				{
					ArmAnimInstance = FPSCharacterArmMesh->GetAnimInstance();
					if (ArmAnimInstance != NULL)
					{
						//ArmAnimInstance->Montage_SetPosition(StartCrankAnimation, 0.23f);
						ArmAnimInstance->Montage_Play(CrankingAnimation, 1.0f);
					}
				}

				CurrentTorch->TorchCrank();

			}
			else if (IsTorchCrankerUp == false)
			{
				if (StartCrankAnimation != NULL)
				{
					ArmAnimInstance = FPSCharacterArmMesh->GetAnimInstance();
					if (ArmAnimInstance != NULL)
					{
						//ArmAnimInstance->Montage_SetPosition(StartCrankAnimation, 0.23f);
						IsTorchCrankerUp = true;
						IsTorchCrancking = true;
						ArmAnimInstance->Montage_Play(StartCrankAnimation, 1.0f);
					}
				}
			}		
			//end animation played using animation blueprint
			//play end inside animation blueprint using notify
		}
	}
}

void ANBCharacter::PowerUpTorch()
{
	if (CurrentTorch)
	{
		CurrentTorch->ActivateTorch();
	}

}

