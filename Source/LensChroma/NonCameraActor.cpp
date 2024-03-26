// Fill out your copyright notice in the Description page of Project Settings.

// The actor that should only be visible when viewing without the camera

#include "NonCameraActor.h"

// Sets default values
ANonCameraActor::ANonCameraActor()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetupAttachment(RootComponent);

}

