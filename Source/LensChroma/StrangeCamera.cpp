// Fill out your copyright notice in the Description page of Project Settings.


#include "StrangeCamera.h"
#include "Components/SphereComponent.h"

// Sets default values
AStrangeCamera::AStrangeCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ColliderSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ColliderSphere"));
	//ColliderSphere->SetupAttachment(RootComponent);
	SetRootComponent(ColliderSphere);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(ColliderSphere);
	StaticMeshComponent->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));

	Material = CreateDefaultSubobject<UMaterialInterface>(TEXT("Material"));
	ConstructorHelpers::FObjectFinder<UMaterialInterface>MaterialAsset(TEXT("Material'/Game/Materials/Camera_M.Camera_M'"));
	//StaticMeshComponent->SetMaterial(0, MaterialAsset.Object);
	Material = MaterialAsset.Object;

}

// Called when the game starts or when spawned
void AStrangeCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStrangeCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//UStaticMeshComponent* AStrangeCamera::CameraInteract_Implementation(AActor* InTarget)
UStaticMeshComponent* AStrangeCamera::CameraGetMesh_Implementation()
{
	return this->StaticMeshComponent;
}

UMaterialInterface* AStrangeCamera::CameraGetMaterial_Implementation()
{
	return this->Material;
}