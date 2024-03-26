// Fill out your copyright notice in the Description page of Project Settings.


#include "CreatureSpawnerBox.h"
#include "Components/SphereComponent.h"

// Sets default values
ACreatureSpawnerBox::ACreatureSpawnerBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CreatureSpawnerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CreatureSpawnerSphere"));
	CreatureSpawnerSphere->SetupAttachment(RootComponent);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMesh->SetupAttachment(CreatureSpawnerSphere);
}
