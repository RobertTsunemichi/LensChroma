// Fill out your copyright notice in the Description page of Project Settings.

// Header
#include "BaseGameMode.h"

// Game instance header
#include "GameInstanceClass.h"

// Gameplay statics functions 
#include <Kismet/GameplayStatics.h>

// Handles to the player character and the strange camera classes
#include "FPSPlayerCharacter.h"
#include "StrangeCamera.h"

/*
 * Things to do when the game begins
 */
void ABaseGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Get a handle to the player character to get the save gmae load function
	AFPSPlayerCharacter* ThePlayer = Cast<AFPSPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	// If the handle was set
	if (ThePlayer)
	{
		// Load the player character state
		if (ThePlayer->LoadPlayer())
		{
			// Get the strange camera
			AStrangeCamera* StrangeCamera = Cast<AStrangeCamera>(UGameplayStatics::GetActorOfClass(GetWorld(), AStrangeCamera::StaticClass()));

			// Get the strange camera mesh and apply it to the player attach position
			ThePlayer->StaticMeshComponentItem->SetStaticMesh(ICameraInteractInterface::Execute_CameraGetMesh(StrangeCamera)->GetStaticMesh());

			// Get the strange camera material and apply it to the player attach position mesh
			ThePlayer->Material = ICameraInteractInterface::Execute_CameraGetMaterial(StrangeCamera);
			ThePlayer->StaticMeshComponentItem->SetMaterial(0, ThePlayer->Material);
		}
	}
}

/*
 * The override function for selecting the player start location
 */
AActor* ABaseGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	// Get the game instance
	UGameInstanceClass* GameInstance = Cast<UGameInstanceClass>(GetGameInstance());

	// Retrieve the tag of the start location we want
	FName TTag = GameInstance->TeleportationTag;

	// Set the start location tag we want if there is no tag set in the game instance
	if (TTag == FName("None")) TTag = FName("TheStartPosition00");

	// Get all the player start locations from the world
	TArray<AActor*> ActorClasses;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), PlayerStartBP, ActorClasses);
	
	// For each actor that was found
	for (AActor* SingleActor : ActorClasses)
	{
		// If the actor has the tag we want, then use this start location
		if (SingleActor->ActorHasTag(TTag)) return SingleActor;
	}

	// Otherwise no start location with the required tag was found. Use the first actor found
	return ActorClasses[0];
}
