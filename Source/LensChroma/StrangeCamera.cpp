// Fill out your copyright notice in the Description page of Project Settings.


#include "StrangeCamera.h"
#include "Components/SphereComponent.h"

// Sets default values
AStrangeCamera::AStrangeCamera()
{
	ColliderSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ColliderSphere"));
	SetRootComponent(ColliderSphere);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(ColliderSphere);
	StaticMeshComponent->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));

	Material = CreateDefaultSubobject<UMaterialInterface>(TEXT("Material"));
	ConstructorHelpers::FObjectFinder<UMaterialInterface>MaterialAsset(TEXT("Material'/Game/Materials/Camera_M.Camera_M'"));
	Material = MaterialAsset.Object;

}

UStaticMeshComponent* AStrangeCamera::CameraGetMesh_Implementation()
{
	return this->StaticMeshComponent;
}

UMaterialInterface* AStrangeCamera::CameraGetMaterial_Implementation()
{
	return this->Material;
}