// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraFilterItem.h"
#include "Components/SphereComponent.h"

// Sets default values
ACameraFilterItem::ACameraFilterItem()
{
	// Set the collider and mesh
	ColliderSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ColliderSphere"));
	SetRootComponent(ColliderSphere);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));

}
