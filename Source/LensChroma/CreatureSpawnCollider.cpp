// Fill out your copyright notice in the Description page of Project Settings.


#include "CreatureSpawnCollider.h"
#include "Components/BoxComponent.h"
#include <Kismet/GameplayStatics.h>
#include "AbrrCreatureCharacterClass.h"
#include "Components/SphereComponent.h"
#include "FPSPlayerCharacter.h"

// Sets default values
ACreatureSpawnCollider::ACreatureSpawnCollider()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ColliderSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ColliderBox"));
	ColliderSphere->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACreatureSpawnCollider::BeginPlay()
{
	Super::BeginPlay();

	ColliderSphere->OnComponentBeginOverlap.AddDynamic(this, &ACreatureSpawnCollider::OnOverlapBegin);

}

// Called every frame
void ACreatureSpawnCollider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Overlap for fleeing
void ACreatureSpawnCollider::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	// FPSCharacter
	AFPSPlayerCharacter* FPSChar = Cast<AFPSPlayerCharacter>(OtherActor);
	if (FPSChar)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT(">>> Removing non-chasing Creatures!")));
		TArray<AActor*> CreatureToRemove;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AberrantCreature, CreatureToRemove);

		if (CreatureToRemove.Num() > 0)
		{
			for (AActor* SingleSpawner : CreatureToRemove)
			{
				AAbrrCreatureCharacterClass* Creature = Cast<AAbrrCreatureCharacterClass>(SingleSpawner);
				if (Creature)
				{
					if (!Creature->bHasPlayerTarget)
					{
						Creature->Destroy();
					}
				}
			}
		}


		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT(">>> Spawning Creatures!")));
		TArray<AActor*> CreatureSpawners;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), CreatureSpawnerBox, CreatureSpawners);

		if (CreatureSpawners.Num() > 0)
		{
			for (AActor* SingleSpawner : CreatureSpawners)
			{
				if (SingleSpawner->ActorHasTag(SpawnerTag))
				{
					AAbrrCreatureCharacterClass* Creature = GetWorld()->SpawnActor<AAbrrCreatureCharacterClass>(AberrantCreature, SingleSpawner->GetActorLocation(), FRotator(0.f, 0.f, 0.f));
					if (Creature)
					{
						if (FPSChar->bIsLookingThroughCamera)
						{
							Creature->GetMesh()->SetMaterial(0, FPSChar->AbbrCreatureMat1);
						}
						else
						{
							Creature->GetMesh()->SetMaterial(0, FPSChar->AbbrCreatureMat2);
						}
					}
				}
			}

			if (bShouldDestroy)
			{
				Destroy();
			}
		}
	}
}