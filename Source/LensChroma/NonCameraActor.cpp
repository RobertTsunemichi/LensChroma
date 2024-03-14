// Fill out your copyright notice in the Description page of Project Settings.

// The actor that should only be visible when viewing without the camera

#include "NonCameraActor.h"

// Sets default values
ANonCameraActor::ANonCameraActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");

}

// Called when the game starts or when spawned
void ANonCameraActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANonCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

