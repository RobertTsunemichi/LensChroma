// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformObject.h"

// Sets default values
APlatformObject::APlatformObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");

}

// Called when the game starts or when spawned
void APlatformObject::BeginPlay()
{
	Super::BeginPlay();

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	
}

// Called every frame
void APlatformObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

