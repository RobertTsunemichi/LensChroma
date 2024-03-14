// Fill out your copyright notice in the Description page of Project Settings.


#include "FollowTargetPoints.h"
#include "Components/SphereComponent.h"

#include "FPSPlayerCharacter.h"

// Sets default values
AFollowTargetPoints::AFollowTargetPoints()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

//// Called every frame
//void AFollowTargetPoints::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//	/*TimeCounter += DeltaTime;
//
//	if (TimeCounter >= 1.0f)
//	{
//		Points -= 1;
//		TimeCounter = 0.f;
//	}*/
//
//}

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