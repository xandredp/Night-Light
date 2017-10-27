// Fill out your copyright notice in the Description page of Project Settings.

#include "SaveGameModule.h"

#include "Engine.h"

USaveGameModule::USaveGameModule()
{
	SaveSlotName = TEXT("DefaultGameSlot");
	UserIndex = 0;
}

void USaveGameModule::SaveGame(ACharacter * ThisCharacter)
{
	ThisCharacter;
	

	FVector Location = ThisCharacter->GetActorLocation();
	Location;

	USaveGameModule* SaveGameInstance = Cast<USaveGameModule>(UGameplayStatics::CreateSaveGameObject(USaveGameModule::StaticClass()));
	SaveGameInstance->PlayerPosition = Location;


	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Saved in c++");


	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SaveSlotName, SaveGameInstance->UserIndex);
}

void USaveGameModule::LoadGame(ACharacter * ThisCharacter)
{
	USaveGameModule* LoadGameInstance = Cast<USaveGameModule>(UGameplayStatics::CreateSaveGameObject(USaveGameModule::StaticClass()));
	LoadGameInstance = Cast<USaveGameModule>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->SaveSlotName, LoadGameInstance->UserIndex));
	FVector PlayerPosition = LoadGameInstance->PlayerPosition;

	ThisCharacter->SetActorLocation(PlayerPosition);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "loaded from c++");
	}
}

