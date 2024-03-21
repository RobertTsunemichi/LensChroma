// Fill out your copyright notice in the Description page of Project Settings.


#include "FollowTargetPoints.h"
#include "Components/SphereComponent.h"

#include "FPSPlayerCharacter.h"

// Sets default values
AFollowTargetPoints::AFollowTargetPoints()
{
	ColliderSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ColliderSphere"));
	SetRootComponent(ColliderSphere);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AFollowTargetPoints::BeginPlay()
{
	Super::BeginPlay();

	ColliderSphere->OnComponentBeginOverlap.AddDynamic(this, &AFollowTargetPoints::OnOverlapBegin);
	
}

// Overlap for fleeing
void AFollowTargetPoints::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	// Check distance to player
	AFPSPlayerCharacter* FPSChar = Cast<AFPSPlayerCharacter>(OtherActor);
	if (FPSChar)
	{
		FPSChar->PlayerScore += Points;
		Destroy();
	}
}