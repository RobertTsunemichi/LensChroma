// Fill out your copyright notice in the Description page of Project Settings.

// Required
#include "BoostUpwardClass.h"

// Collider
#include "Components/SphereComponent.h"

// Check for this player
#include "FPSPlayerCharacter.h"

/*
 * Constructor
 */
ABoostUpwardClass::ABoostUpwardClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Collider
	ColliderSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ColliderSphere"));
	SetRootComponent(ColliderSphere);

	// Static Mesh
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(GetRootComponent());

	// Default boost strength
	BoostStrength = 1500.f;

}

/*
 * Set the actor state on begin
 */
void ABoostUpwardClass::BeginPlay()
{
	Super::BeginPlay();

	ColliderSphere->OnComponentBeginOverlap.AddDynamic(this, &ABoostUpwardClass::OnOverlapBegin);

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	
}

/*
 * Boost the player on collider sphere overlap
 */
void ABoostUpwardClass::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	AFPSPlayerCharacter* FPSChar = Cast<AFPSPlayerCharacter>(OtherActor);
	if (FPSChar)
	{
		// Set the launch vector
		FVector FPSDirection = FVector(
			FPSChar->GetVelocity().X, 
			FPSChar->GetVelocity().Y, 
			FPSChar->GetVelocity().Z + BoostStrength);

		// Launch the character, overriding the x, y, and z
		FPSChar->LaunchCharacter(FPSDirection, true, true);
	}
	// else do nothing
}