// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformObject.h"

// Sets default values
APlatformObject::APlatformObject()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void APlatformObject::BeginPlay()
{
	Super::BeginPlay();

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	
}

