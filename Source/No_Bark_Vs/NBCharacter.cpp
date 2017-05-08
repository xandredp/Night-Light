// Fill out your copyright notice in the Description page of Project Settings.

#include "No_Bark_Vs.h"
#include "BaseInteractable.h"
#include "PlayController.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "Engine.h"
#include "NBCharacter.h"


ANBCharacter::ANBCharacter()
{

	walkingSpeed = 400.0f;
	MaxSprintSpeed = 600.0f;


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

	// Item

	MaxInteractDistance = 500.0f;
}

void ANBCharacter::BeginPlay()
{

	Super::BeginPlay();

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
	/*PlayerInputComponent->BindAction("Targeting", IE_Pressed, this, &ANBCharacter::OnStartTargeting);
	PlayerInputComponent->BindAction("Targeting", IE_Released, this, &ANBCharacter::OnEndTargeting);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ANBCharacter::OnStartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ANBCharacter::OnStopFire);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ANBCharacter::OnReload);

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
		AttachEquipmentToHand();
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "WeaponClass  Is somnething.");
	}
}


void ANBCharacter::GetEquipment(int index)
{
	APlayController* playController = Cast<APlayController>(GetController());
	if (playController)
	{
		if (playController->FCurrentEquippedWeapons.Num() != 0)
		{
			FCurrentInventoryItemInfo WeaponToEquipData = playController->FCurrentEquippedWeapons[0];
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


void ANBCharacter::DecreaseHealth(float decreaseVal)
{
}

void ANBCharacter::IncreaseHealth(float increaseVal)
{
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

	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f);

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
	MoveComp->MaxWalkSpeed = MaxSprintSpeed;
	if (Role == ROLE_Authority)
	{
		// Set a timer to increment hunger every interval
		GetWorldTimerManager().SetTimer(StartSprintingTimerHandle, this, &ANBCharacter::DecreaseStamina, StaminaTimerRate, true);
		GetWorldTimerManager().ClearTimer(StopSprintingTimerHandle);
	}
}

void ANBCharacter::OnStopSprinting()
{
	MoveComp->MaxWalkSpeed = walkingSpeed;

	if (Role == ROLE_Authority)
	{
		// Set a timer to increment hunger every interval
		GetWorldTimerManager().SetTimer(StopSprintingTimerHandle, this, &ANBCharacter::IncreaseStamina, StaminaTimerRate, true);
		GetWorldTimerManager().ClearTimer(StartSprintingTimerHandle);
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