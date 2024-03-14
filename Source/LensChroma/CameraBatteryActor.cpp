// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraBatteryActor.h"
#include "Components/SphereComponent.h"

// Sets default values
ACameraBatteryActor::ACameraBatteryActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ColliderSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ColliderSphere"));
	SetRootComponent(ColliderSphere);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ACameraBatteryActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACameraBatteryActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

