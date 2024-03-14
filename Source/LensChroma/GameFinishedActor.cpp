// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFinishedActor.h"
#include "Components/SphereComponent.h"
#include "FPSPlayerCharacter.h"
#include <Kismet/GameplayStatics.h>
#include "AbrrCreatureCharacterClass.h"

// Sets default values
AGameFinishedActor::AGameFinishedActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
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

// Called every frame
void AGameFinishedActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
		//GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, FString::Printf(TEXT("Has Finished!!!")));
		// 
		if (!FPSChar->bHasFinished)
		{
			FPSChar->PlayerScore += (FPSChar->PlayerSanity / 100.f) * 10000;
			FPSChar->PlayerScore += FPSChar->CameraBattery * 10;
			FPSChar->bHasFinished = true;
		}

		//GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, FString::Printf(TEXT("Removing hostiles")));
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