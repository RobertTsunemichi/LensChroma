// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameMode.h"
#include "GameInstanceClass.h"
#include <Kismet/GameplayStatics.h>

#include "FPSPlayerCharacter.h"
#include "StrangeCamera.h"
#include "CameraLens.h"
#include "Camera/CameraComponent.h"

void ABaseGameMode::BeginPlay()
{
	Super::BeginPlay();

	AFPSPlayerCharacter* ThePlayer = Cast<AFPSPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (ThePlayer)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("BGM Loading Player")));
		if (ThePlayer->LoadPlayer())
		{
			// Get the strange camera mesh and material
			AStrangeCamera* StrangeCamera = Cast<AStrangeCamera>(UGameplayStatics::GetActorOfClass(GetWorld(), AStrangeCamera::StaticClass()));

			// Get the strange camera mesh and apply it to the player attach position
			ThePlayer->StaticMeshComponentItem->SetStaticMesh(ICameraInteractInterface::Execute_CameraGetMesh(StrangeCamera)->GetStaticMesh());

			// Get the strange camera material and apply it to the player attach position mesh
			ThePlayer->Material = ICameraInteractInterface::Execute_CameraGetMaterial(StrangeCamera);
			ThePlayer->StaticMeshComponentItem->SetMaterial(0, ThePlayer->Material);
		}
	}
}

AActor* ABaseGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	UGameInstanceClass* GameInstance = Cast<UGameInstanceClass>(GetGameInstance());
	FName TTag = GameInstance->TeleportationTag;

	if (TTag == FName("None"))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Got TTag:None, should set a default player start tag.")));
		TTag = FName("TheStartPosition00");
	}

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Teletag DBG: %s"), *TTag.ToString()));

	TArray<AActor*> ActorClasses;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), PlayerStartBP, ActorClasses);

	for (AActor* SingleActor : ActorClasses)
	{
		if (SingleActor->ActorHasTag(TTag))
		{
			return SingleActor;
		}
	}

	ActorClasses[0]->Tags.Add(FName("FAILURE")); // DEBUG, Can remove this
	return ActorClasses[0];
}

void ABaseGameMode::InitPlayerSavedData()
{
	if (UGameplayStatics::DoesSaveGameExist("PlayerData", 0))
	{
		UGameplayStatics::LoadGameFromSlot("PlayerData", 0);

		// Level info processing
		SavePlayerObject = Cast<USaveGamePlayer>(UGameplayStatics::LoadGameFromSlot("PlayerData", 0));
	}
	else
	{
		UGameplayStatics::CreateSaveGameObject(PlayerSaveData);
	}
}