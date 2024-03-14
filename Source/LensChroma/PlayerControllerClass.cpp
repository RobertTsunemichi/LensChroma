// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerClass.h"
#include "Blueprint/UserWidget.h"

void APlayerControllerClass::BeginPlay()
{
	Super::BeginPlay();

	if (HUDOverlayAsset)
	{
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
		HUDOverlay->AddToViewport();
		HUDOverlay->SetVisibility(ESlateVisibility::Visible);
	}

	if (WPauseMenu)
	{
		PauseMenu = CreateWidget<UUserWidget>(this, WPauseMenu);

		if (PauseMenu)
		{
			PauseMenu->AddToViewport();
			PauseMenu->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void APlayerControllerClass::DisplayPauseMenu_Implementation()
{
	if (PauseMenu)
	{
		SetPause(true);

		bPauseMenuVisible = true;

		PauseMenu->SetVisibility(ESlateVisibility::Visible);

		FInputModeUIOnly InputModeUIOnly;
		SetInputMode(InputModeUIOnly);

		bShowMouseCursor = true;
	}
}

void APlayerControllerClass::RemovePauseMenu_Implementation()
{
	if (PauseMenu)
	{
		PauseMenu->SetVisibility(ESlateVisibility::Hidden);

		bShowMouseCursor = false;
		bPauseMenuVisible = false;

		FInputModeGameOnly InputModeGameOnly;
		SetInputMode(InputModeGameOnly);

		SetPause(false);
	}
}

void APlayerControllerClass::TogglePauseMenu()
{
	if (bPauseMenuVisible)
	{
		RemovePauseMenu();
	}
	else
	{
		DisplayPauseMenu();
	}
}