// Fill out your copyright notice in the Description page of Project Settings.

#include "No_Bark_Vs.h"
#include "BaseInteractable.h"
#include "PlayController.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "Engine.h"
#include "NBCharacter.h"


ANBCharacter::ANBCharacter()
{

	ABaseWeapon* aBaseWeapon = Cast<ABaseWeapon>(WeaponClass);
	WeaponClass->IsChildOf(ABaseWeapon::StaticClass());

	WeaponClass = ABaseWeapon::StaticClass();
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	SpeedValue = 4.0;

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
	KnifeAttachPoint = TEXT("Thigh_Socket");
	PrimaryAttachPoint = TEXT("Spine_Socket");
	SecondaryAttachPoint = TEXT("Clavicle_Socket");
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

	InputComponent->BindAction("CrouchToggle", IE_Released, this, &ANBCharacter::OnCrouchToggle);

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
	//Trace with Raycasting Everytick and check the item is interactable. 
	FHitResult HitResult;

	FVector StartTrace = FollowCamera->GetComponentLocation();
	FVector EndTrace = (FollowCamera->GetForwardVector() * 300) + StartTrace;

	//Add To ignore these items for checking interactable
	FCollisionQueryParams QuerryParams;
	QuerryParams.AddIgnoredActor(this);

	APlayController* playController = Cast<APlayController>(GetController());
	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Visibility, QuerryParams) && Controller)
	{
		if(ABaseInteractable* interactable = Cast<ABaseInteractable>(HitResult.GetActor()))
		{
			playController->CurrentInteractable = interactable;
			return;
		}
	}

	// If we didn't hit anything, or thing we hit was on to a interactable set current interactable nullptr. 
	playController->CurrentInteractable = nullptr;
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
	
		if (bWantsToRun== true)
		{
			AddMovementInput(Direction, SpeedValue*Value);
		}
		else
		{
			AddMovementInput(Direction, Value);
		}
		
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
		
		if (bWantsToRun == true)
		{
			AddMovementInput(Direction, SpeedValue*Value);
		}
		else
		{
			AddMovementInput(Direction, Value);
		}
	}
}

void ANBCharacter::OnStartSprinting()
{
	SetSprinting(true);
}

void ANBCharacter::OnStopSprinting()
{
	SetSprinting(false);
}

void ANBCharacter::SetSprinting(bool NewSprinting)
{

	bWantsToRun = NewSprinting;

	//If wanting to sprinting stop fire and uncrouch
	if (bWantsToRun)
	{
		//StopWeaponFire();
		//if (bIsCrouched)
		//	UnCrouch();
	}

}

void ANBCharacter::OnCrouchToggle()
{
	


	// If we are crouching then CanCrouch will return false. If we cannot crouch then calling Crouch() wont do anything
	if (CanCrouch())
	{
		Crouch();
	}
	else
	{
		UnCrouch();
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
	case EInventorySlot::Knife:
		return KnifeAttachPoint;
	default:
		// Not implemented.
		return "";
	}
}