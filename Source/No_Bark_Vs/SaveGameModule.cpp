// Fill out your copyright notice in the Description page of Project Settings.

#include "SaveGameModule.h"
#include "GameFramework/Character.h"
#include "Monsters/Monster Types/NBSkinnyAI.h"
#include "Player/NBCharacter.h"
//#include "Player/PlayController.h"
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
		GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Orange, "Weapon Saved");
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
		GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Orange, "Torch Saved");
		SaveGameInstance->HasTorch = true;
	}
	else
	{
		SaveGameInstance->HasTorch = false;
	}


	UWorld* World = ThisCharacter->GetWorld();

	for (TActorIterator<ANBBaseAI> It(World); It; ++It)
	{

		if (It->GetMonsterDead()) 
		{
			GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Orange, "Monster Dead");
		}
		else
		{ 		
			//GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Blue, It->GetFullName());
			SaveGameInstance->Zombies.Add(It->GetFullName());
		}
	}
	

	APlayController* Controller = Cast<APlayController>(World->GetFirstPlayerController());

	SaveGameInstance->Hours = Controller->Hours;
	SaveGameInstance->Minutes = Controller->Minutes;
	SaveGameInstance->Seconds = Controller->Seconds;

	SaveGameInstance->CurrentPossesedBatteries = Controller->CurrentPossesedBatteries;
	SaveGameInstance->CurrentPossesedBattery = Controller->CurrentPossesedBattery;

	SaveGameInstance->CurrentPossesedKeys = Controller->CurrentPossesedKeys;
	SaveGameInstance->CurrentPossesedKey = Controller->CurrentPossesedKey;



	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SaveSlotName, SaveGameInstance->UserIndex);


	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "Saved to Slot in c++");
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


	//nb->WeaponClass = LoadGameInstance->WeaponClass;

	if (LoadGameInstance->HasWeapon)
	{	
		GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Green, "Has Weapon");
		nb->EquipPrimaryWeapon();
		nb->CurrentWeapon->CurrentClip = LoadGameInstance->CurrentClip;
		nb->CurrentWeapon->CurrentAmmo = LoadGameInstance->CurrentAmmo;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Green, "No Weapon loaded");
	}


	if (LoadGameInstance->HasTorch)
	{
		GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Emerald, "Has Torch");
		nb->SpawnTorch();
	}
	
	UWorld* World = ThisCharacter->GetWorld();
	for (TActorIterator<ANBBaseAI> It(World); It; ++It)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Blue, It->GetFullName());

		int32 Idx;

		if (LoadGameInstance->Zombies.Find(It->GetFullName(), Idx))
		{

		}
		else
		{			
			GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Green, It->GetFullName());
			GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Green, "   Not found in save so Destroying" );
			It->Destroy();
		}
	}

	APlayController* Controller = Cast<APlayController>(World->GetFirstPlayerController());

	 Controller->Hours = LoadGameInstance->Hours;
	 Controller->Minutes = LoadGameInstance->Minutes;
	 Controller->Seconds= LoadGameInstance->Seconds;

	 Controller->CurrentPossesedBatteries = LoadGameInstance->CurrentPossesedBatteries;
	 Controller->CurrentPossesedBattery = LoadGameInstance->CurrentPossesedBattery;

	 Controller->CurrentPossesedKeys = LoadGameInstance->CurrentPossesedKeys;
	 Controller->CurrentPossesedKey = LoadGameInstance->CurrentPossesedKey;


	 GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Green, FString::Printf(TEXT("Batteries count %d"), Controller->CurrentPossesedBatteries.Num()));
	 GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Green, FString::Printf(TEXT("Keys count %d"), Controller->CurrentPossesedKeys.Num()));


	 //nb->dead

	if (GEngine)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "Loaded from c++");
	}
}

