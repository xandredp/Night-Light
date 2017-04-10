// Fill out your copyright notice in the Description page of Project Settings.

#include "No_Bark_Vs.h"
#include "BaseInteractable.h"
#include "PlayController.h"




void APlayController::Interact()
{
	if (CurrentInteractable)
	{
		CurrentInteractable->Interact(this);

	}
}

void APlayController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Use", IE_Pressed, this, &APlayController::Interact);


}
