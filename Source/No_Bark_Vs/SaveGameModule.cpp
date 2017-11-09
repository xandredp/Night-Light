// Fill out your copyright notice in the Description page of Project Settings.

#include "SaveGameModule.h"
#include "GameFramework/Character.h"
#include "Monsters/Monster Types/NBSkinnyAI.h"
#include "Player/NBCharacter.h"
#include "Player/PlayController.h"
#include "Core/BaseWeapon.h"

#include "Engine.h"

USaveGameModule::USaveGameModule()
{
	SaveSlotName = TEXT("DefaultGameSlot");
	UserIndex = 0;
}

void USaveGameModule::SaveGame(ACharacter * ThisCharacter)
{
	//ThisCharacter;
	
	FVector Location = ThisCharacter->GetActorLocation();
	//Location;

	USaveGameModule* SaveGameInstance = Cast<USaveGameModule>(UGameplayStatics::CreateSaveGameObject(USaveGameModule::StaticClass()));
	SaveGameInstance->PlayerPosition = Location;

	ANBCharacter* nb = Cast<ANBCharacter>(ThisCharacter);

	SaveGameInstance->Health = nb->CurrentHealth;
	SaveGameInstance->CurrentScore = nb->CurrentScore;
	SaveGameInstance->MaxHealth = nb->MaxHealth;
	SaveGameInstance->KillCount = nb->MonsterKillCount;

	SaveGameInstance->WeaponClass = nb->WeaponClass;
	SaveGameInstance->TorchClass = nb->TorchClass;

	if (nb->CurrentWeapon)
	{
		SaveGameInstance->HasWeapon = true;
		SaveGameInstance->CurrentClip = nb->CurrentWeapon->CurrentClip;
		SaveGameInstance->CurrentAmmo = nb->CurrentWeapon->CurrentAmmo;
	}
	else
	{
		SaveGameInstance->HasWeapon = false;
	}


	if (nb->CurrentTorch)
	{
		SaveGameInstance->HasTorch = true;
	}
	else
	{
		SaveGameInstance->HasTorch = false;
	}

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Saved in c++");

	UWorld* World = ThisCharacter->GetWorld();

	for (TActorIterator<ANBBaseAI> It(World); It; ++It)
	{

		if (It->GetMonsterDead()) 
		{
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, "Monster Dead");
		}
		else
		{ 		
			//GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Blue, It->GetFullName());
			SaveGameInstance->Zombies.Add(It->GetFullName());
		}
	}
	

	APlayController* Controller = Cast<APlayController>(World->GetFirstPlayerController());

	SaveGameInstance->Hours = Controller->Hours;
	SaveGameInstance->Minutes = Controller->Minutes;
	SaveGameInstance->Seconds = Controller->Seconds;

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SaveSlotName, SaveGameInstance->UserIndex);
}

void USaveGameModule::LoadGame(ACharacter * ThisCharacter)
{
	USaveGameModule* LoadGameInstance = Cast<USaveGameModule>(UGameplayStatics::CreateSaveGameObject(USaveGameModule::StaticClass()));
	LoadGameInstance = Cast<USaveGameModule>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->SaveSlotName, LoadGameInstance->UserIndex));

	// Reset the player position
	FVector PlayerPosition = LoadGameInstance->PlayerPosition;
	ThisCharacter->SetActorLocation(PlayerPosition);

	ANBCharacter* nb = Cast<ANBCharacter>(ThisCharacter);

	// reload the basic NBCharacter settings
	nb->CurrentHealth = LoadGameInstance->Health;
	nb->CurrentScore = LoadGameInstance->CurrentScore;
	nb->MaxHealth = LoadGameInstance->MaxHealth;
    nb->MonsterKillCount = LoadGameInstance->KillCount;


	nb->WeaponClass = LoadGameInstance->WeaponClass;

	if (LoadGameInstance->HasWeapon)
	{	
		nb->EquipPrimaryWeapon();
		nb->CurrentWeapon->CurrentClip = LoadGameInstance->CurrentClip;
		nb->CurrentWeapon->CurrentAmmo = LoadGameInstance->CurrentAmmo;
	}
	if (LoadGameInstance->HasTorch)
	{
		nb->SpawnTorch();
	}
	
	UWorld* World = ThisCharacter->GetWorld();
	for (TActorIterator<ANBBaseAI> It(World); It; ++It)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, It->GetFullName());

		int32 Idx;

		if (LoadGameInstance->Zombies.Find(It->GetFullName(), Idx))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Green, It->GetFullName());
		}
		else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, "Not found in save so Destroying" );
			It->Destroy();
		}
	}

	APlayController* Controller = Cast<APlayController>(World->GetFirstPlayerController());

	 Controller->Hours = LoadGameInstance->Hours;
	 Controller->Minutes = LoadGameInstance->Minutes;
	 Controller->Seconds= LoadGameInstance->Seconds;

	if (GEngine)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "Loaded from c++");
	}
}

