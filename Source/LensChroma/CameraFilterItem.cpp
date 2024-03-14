// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraFilterItem.h"
#include "Components/SphereComponent.h"

// Sets default values
ACameraFilterItem::ACameraFilterItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ColliderSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ColliderSphere"));
	SetRootComponent(ColliderSphere);
	//ColliderSphere->SetupAttachment(StaticMeshComponent);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));

	Material = CreateDefaultSubobject<UMaterialInterface>(TEXT("Material"));
	// Get the correct material, instead of: Material'/Game/Materials/Camera_M.Camera_M'
	ConstructorHelpers::FObjectFinder<UMaterialInterface>MaterialAsset(TEXT("Material'/Game/Materials/Camera_M.Camera_M'"));
	Material = MaterialAsset.Object;

}

// Called when the game starts or when spawned
void ACameraFilterItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACameraFilterItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

