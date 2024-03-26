// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFinishedActor.h"
#include "Components/SphereComponent.h"
#include "FPSPlayerCharacter.h"
#include <Kismet/GameplayStatics.h>
#include "AbrrCreatureCharacterClass.h"

// Sets default values
AGameFinishedActor::AGameFinishedActor()
{	
	ColliderSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ColliderSphere"));
	ColliderSphere->SetupAttachment(RootComponent);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(ColliderSphere);

}

// Called when the game starts or when spawned
void AGameFinishedActor::BeginPlay()
{
	Super::BeginPlay();
	
	ColliderSphere->OnComponentBeginOverlap.AddDynamic(this, &AGameFinishedActor::OnOverlapBegin);
}

// Overlap for fleeing
void AGameFinishedActor::OnOverlapBegin(
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
		// 
		if (!FPSChar->bHasFinished)
		{
			FPSChar->PlayerScore += (FPSChar->PlayerSanity / 100.f) * 10000;
			FPSChar->PlayerScore += FPSChar->CameraBattery * 10;
			FPSChar->bHasFinished = true;
		}

		// Remove hostiles
		TArray<AActor*> ActorsToRemove;
		// Toggle platform objects
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AbrrCreatureObject, ActorsToRemove);

		for (AActor* ActorToRemove : ActorsToRemove)
		{
			AAbrrCreatureCharacterClass* SinglePObject = Cast<AAbrrCreatureCharacterClass>(ActorToRemove);
			if (SinglePObject)
			{
				SinglePObject->Destroy();
			}
		}
	}
}