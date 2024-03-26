// Fill out your copyright notice in the Description page of Project Settings.

#include "PortalActor.h"
#include "Components/SphereComponent.h"
#include "GameInstanceClass.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
APortalActor::APortalActor()
{
	ColliderSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ColliderSphere"));
	SetRootComponent(ColliderSphere);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void APortalActor::BeginPlay()
{
	Super::BeginPlay();

	// Set the default state of the object to be hidden and not collide-able
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

void APortalActor::ActivateInteraction_Implementation(AActor* InActor, int InInt)
{
	// Open the level
	UGameInstanceClass* GameInstance = Cast<UGameInstanceClass>(GetGameInstance());
	GameInstance->TeleportationTag = TeleTag;
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}