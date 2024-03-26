// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraBatteryActor.h"
#include "Components/SphereComponent.h"

// Sets default values
ACameraBatteryActor::ACameraBatteryActor()
{
	ColliderSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ColliderSphere"));
	SetRootComponent(ColliderSphere);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

}
