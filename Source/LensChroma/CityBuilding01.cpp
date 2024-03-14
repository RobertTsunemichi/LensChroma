// Fill out your copyright notice in the Description page of Project Settings.


#include "CityBuilding01.h"

// Sets default values
ACityBuilding01::ACityBuilding01()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create and set the default mesh
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(StaticMeshComponent);

}

// Called when the game starts or when spawned
void ACityBuilding01::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACityBuilding01::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

