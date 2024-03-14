// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalActor.h"
#include "Components/SphereComponent.h"
#include "FPSPlayerCharacter.h"
#include "GameInstanceClass.h"
#include <Kismet/GameplayStatics.h>
#include "StrangeCamera.h"

// Sets default values
APortalActor::APortalActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ColliderSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ColliderSphere"));
	SetRootComponent(ColliderSphere);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void APortalActor::BeginPlay()
{
	Super::BeginPlay();

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

// Called every frame
void APortalActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// TODO: remove the AActor* requirement in the input parameters
void APortalActor::ActivateInteraction_Implementation(AActor* InActor, int InInt)
{
	// DEBUG
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("Activated Interactable: %d, %s"), InInt, *FName("Test").ToString()));

	// This section isn't really needed.
	AFPSPlayerCharacter* FPSChar = Cast<AFPSPlayerCharacter>(InActor);
	if (FPSChar)
	{
		//FPSChar->CurrentActiveLevel = LevelName;
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("Got FPS Character!")));
	}

	// Open the level
	UGameInstanceClass* GameInstance = Cast<UGameInstanceClass>(GetGameInstance());
	GameInstance->TeleportationTag = TeleTag;
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}