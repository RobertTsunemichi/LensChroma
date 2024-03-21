// Fill out your copyright notice in the Description page of Project Settings.


#include "SanityPillActor.h"
#include "Components/SphereComponent.h"

// Sets default values
ASanityPillActor::ASanityPillActor()
{
	ColliderSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ColliderSphere"));
	SetRootComponent(ColliderSphere);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

}

