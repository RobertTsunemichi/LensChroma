// Fill out your copyright notice in the Description page of Project Settings.


#include "SanityPillActor.h"
#include "Components/SphereComponent.h"

// Sets default values
ASanityPillActor::ASanityPillActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ColliderSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ColliderSphere"));
	SetRootComponent(ColliderSphere);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ASanityPillActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASanityPillActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

