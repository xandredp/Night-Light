// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/No_Bark_Vs.h"
#include "Core/BaseInteractable.h"
#include "Player/PlayController.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "Engine.h"
#include "Player/NBCharacter.h"


ANBCharacter::ANBCharacter()
{

	walkingSpeed = 400.0f;
	MaxSprintSpeed = 600.0f;

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

	ABaseWeapon* aBaseWeapon = Cast<ABaseWeapon>(WeaponClass);
	WeaponClass->IsChildOf(ABaseWeapon::StaticClass());
	WeaponClass = ABaseWeapon::StaticClass();

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

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

												// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

												   // Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
												   // are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
												   /* Names as specified in the character skeleton */
	



	WeaponAttachPoint = TEXT("Weapon_Socket");
	MeleeAttachPoint = TEXT("Thigh_Socket");
	PrimaryAttachPoint = TEXT("Spine_Socket");
	SecondaryAttachPoint = TEXT("Clavicle_Socket");

	
	//Status
	CurrentHealth = 100.0f;
	MaxHealth = 100.0f;
	CurrentStamina = 100.0f;
	MaxStamina = 100.0f;
	CurrentMagic = 100.0f;
	MaxMagic = 100.0f;
	StaminaRegenRate = 1.0f;
	SprintDeductionRate = 1.3f;
	StaminaTimerRate = 0.5f;
	HealthTimerRate = 1.0f;
	MagicTimerRate = 1.0f;
	bIsFiring = false;
	// Item

	MaxInteractDistance = 500.0f;
}

void ANBCharacter::BeginPlay()
{
	Super::BeginPlay();

	AnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	bIsDead = false;

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
}

void ANBCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("SprintHold", IE_Pressed, this, &ANBCharacter::OnStartSprinting);
	PlayerInputComponent->BindAction("SprintHold", IE_Released, this, &ANBCharacter::OnStopSprinting);

	PlayerInputComponent->BindAction("CrouchToggle", IE_Released, this, &ANBCharacter::OnCrouchToggle);
	PlayerInputComponent->BindAction("PrimaryWeapon", IE_Pressed, this, &ANBCharacter::EquipPrimaryWeapon);
	PlayerInputComponent->BindAction("ScondaryWeapon", IE_Pressed, this, &ANBCharacter::EquipSecondaryWeapon);
	PlayerInputComponent->BindAction("Spawn", IE_Pressed, this, &ANBCharacter::Spawn);

	PlayerInputComponent->BindAxis("MoveForward", this, &ANBCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ANBCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ANBCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ANBCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ANBCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ANBCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ANBCharacter::OnResetVR);

	// Weapons


	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ANBCharacter::FireWeapon);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ANBCharacter::StopFireWeapon);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ANBCharacter::ReloadWeapon);
	/*PlayerInputComponent->BindAction("Targeting", IE_Pressed, this, &ANBCharacter::OnStartTargeting);
	PlayerInputComponent->BindAction("Targeting", IE_Released, this, &ANBCharacter::OnEndTargeting);

	
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ANBCharacter::OnStopFire);



	PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, this, &ANBCharacter::OnNextWeapon);
	PlayerInputComponent->BindAction("PrevWeapon", IE_Pressed, this, &ANBCharacter::OnPrevWeapon);

	PlayerInputComponent->BindAction("EquipPrimaryWeapon", IE_Pressed, this, &ANBCharacter::OnEquipPrimaryWeapon);
	PlayerInputComponent->BindAction("EquipSecondaryWeapon", IE_Pressed, this, &ANBCharacter::OnEquipSecondaryWeapon);
*/



}

void ANBCharacter::EquipPrimaryWeapon()
{
	GetEquipment(0);
	
	
	if (WeaponClass == NULL)
	{			
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "WeaponClassIsEmpty");
	}
	else
	{
		AnimInstance->IsPrimaryEquiped = true;
		SpawnWeapon(WeaponClass);
		AttachEquipmentToHand();
	}
}
void ANBCharacter::EquipSecondaryWeapon()
{
	GetEquipment(1);

	if (WeaponClass == NULL)
	{

		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "WeaponClassIsEmpty");
	}
	else
	{
		SpawnWeapon(WeaponClass);

		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "WeaponClass  Is somnething.");
	}
	AttachEquipmentToHand();
}
void ANBCharacter::EquipMeleeWeapon()
{
	APlayController* playController = Cast<APlayController>(GetController());
	if (playController)
	{
		if (playController->FCurrentEquippedMeleeWeapon.CurrentStackNumber >0)
		{
			FCurrentInventoryItemInfo WeaponToEquipData = playController->FCurrentEquippedMeleeWeapon;
			WeaponClass = WeaponToEquipData.ItemInfo.ItemWeaponClass;
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "wehavesomethingto Assign");
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Fail");
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Failusingplayercontrooller");
	}

	if (WeaponClass == NULL)
	{

		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "WeaponClassIsEmpty");
	}
	else
	{
		SpawnWeapon(WeaponClass);

		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "WeaponClass  Is somnething.");
	}
}
void ANBCharacter::EquipOthers(int i_SlotNumber)
{
	APlayController* playController = Cast<APlayController>(GetController());
	if (playController)
	{
		if (playController->FCurrentEquipment.Num() != 0)
		{
			FCurrentInventoryItemInfo WeaponToEquipData = playController->FCurrentEquipment[i_SlotNumber];
			WeaponClass = WeaponToEquipData.ItemInfo.ItemWeaponClass;
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "wehavesomethingto Assign");
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Fail");
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Failusingplayercontrooller");
	}

	if (WeaponClass == NULL)
	{

		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "WeaponClassIsEmpty");
	}
	else
	{
		SpawnWeapon(WeaponClass);

		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "WeaponClass  Is somnething.");
	}
}

void ANBCharacter::Spawn()
{
	if (Spawner != NULL)
	{
		Spawner->SpawnMonster();
	}
}

void ANBCharacter::GetEquipment(int index)
{
	APlayController* playController = Cast<APlayController>(GetController());
	if (playController)
	{
		if (playController->FCurrentEquippedWeapons.Num() >= index)
		{
			FCurrentInventoryItemInfo WeaponToEquipData = playController->FCurrentEquippedWeapons[index];
			WeaponClass = WeaponToEquipData.ItemInfo.ItemWeaponClass;
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "wehavesomethingto Assign");
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Fail");
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Failusingplayercontrooller");
	}
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
	}
	else
	{
		CurrentWeapon->Destroy();
		CurrentWeapon = GetWorld()->SpawnActor<ABaseWeapon>(iWeaponClass, SpawnParams);
	}
	CurrentWeapon->SetOwningPawn(this);
}

void ANBCharacter::SpawnWeaponOnSlot(TSubclassOf<class ABaseWeapon> iWeaponClass, EInventorySlot EquipSlot)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;

	USkeletalMeshComponent* PawnMesh = GetMesh();
	// if current weapon is empty assign current weapon
	if (EquipSlot== EInventorySlot::Primary)
	{
		if (PrimaryWeapon == nullptr)
		{
			PrimaryWeapon = GetWorld()->SpawnActor<ABaseWeapon>(iWeaponClass, SpawnParams);

			PrimaryWeapon->SetOwningPawn(this);
			PrimaryWeapon->WeaponMesh->AttachToComponent(PawnMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, PrimaryAttachPoint);

			//Detach from pawn to equip in hand
			PrimaryWeapon->WeaponMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
			PrimaryWeapon->WeaponMesh->SetHiddenInGame(true);
			EquipPrimaryWeapon();
		}
		else
		{
			PrimaryWeapon->Destroy();
			PrimaryWeapon = GetWorld()->SpawnActor<ABaseWeapon>(iWeaponClass, SpawnParams);

			PrimaryWeapon->SetOwningPawn(this);
			PrimaryWeapon->WeaponMesh->AttachToComponent(PawnMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, PrimaryAttachPoint);

		}
	}
	else if (EquipSlot == EInventorySlot::Secondary)
	{
		if (SecondaryWeapon == nullptr)
		{
			SecondaryWeapon = GetWorld()->SpawnActor<ABaseWeapon>(iWeaponClass, SpawnParams);
			SecondaryWeapon->SetOwningPawn(this);
			SecondaryWeapon->WeaponMesh->AttachToComponent(PawnMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SecondaryAttachPoint);

		}
		else
		{
			SecondaryWeapon->Destroy();
			SecondaryWeapon = GetWorld()->SpawnActor<ABaseWeapon>(iWeaponClass, SpawnParams);
			SecondaryWeapon->SetOwningPawn(this);
			SecondaryWeapon->WeaponMesh->AttachToComponent(PawnMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SecondaryAttachPoint);

		}
		SecondaryWeapon->SetOwningPawn(this);
	}
	else if (EquipSlot == EInventorySlot::Pistol)
	{
		
	}
	else if (EquipSlot == EInventorySlot::Melee)
	{

	}

}
	
void ANBCharacter::DecreaseHealth(float decreaseVal)
{
	
	if (CurrentHealth > 0)
	{
		CurrentHealth -= decreaseVal;	
	}
	else
	{
		bIsDead = true;
		DestroyAndBackToMenu();
	}
}

void ANBCharacter::IncreaseHealth(float increaseVal)
{
	CurrentHealth += increaseVal;
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

void ANBCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
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
	playController->CurrentInteractable = nullptr;
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
	}
}
void ANBCharacter::OnStartSprinting()
{
	if (bIsFiring == false)
	{
		MoveComp->MaxWalkSpeed = MaxSprintSpeed;
		if (Role == ROLE_Authority)
		{
			GetWorldTimerManager().SetTimer(StartSprintingTimerHandle, this, &ANBCharacter::DecreaseStamina, StaminaTimerRate, true);
			GetWorldTimerManager().ClearTimer(StopSprintingTimerHandle);
			UCharacterAnimInstance* AnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance());
			AnimInstance->IsSprinting = true;
		}
	}
}
void ANBCharacter::OnStopSprinting()
{
	if (bIsFiring == false)
	{
		MoveComp->MaxWalkSpeed = walkingSpeed;

		if (Role == ROLE_Authority)
		{

			GetWorldTimerManager().SetTimer(StopSprintingTimerHandle, this, &ANBCharacter::IncreaseStamina, StaminaTimerRate, true);
			GetWorldTimerManager().ClearTimer(StartSprintingTimerHandle);
			UCharacterAnimInstance* AnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance());
			AnimInstance->IsSprinting = false;
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
void ANBCharacter::FireWeapon()
{
	bIsFiring = true;
	if (CurrentWeapon)
	{
		CurrentWeapon->SetTimerForFiring();
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
		CurrentWeapon->ReloadAmmo();
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