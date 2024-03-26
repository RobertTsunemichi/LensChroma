// Fill out your copyright notice in the Description page of Project Settings.

// Required header
#include "CreatureSpawnCollider.h"

// Gameplay static header
#include <Kismet/GameplayStatics.h>

// Collider sphere header
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Spawn behaviour targets
#include "AbrrCreatureCharacterClass.h"
#include "FPSPlayerCharacter.h"

/*
 * Constructor
 */
ACreatureSpawnCollider::ACreatureSpawnCollider()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Collider sphere set up
	ColliderSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AbrrSpawnColliderSphere"));
	ColliderSphere->SetupAttachment(RootComponent);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMesh->SetupAttachment(ColliderSphere);
}

// Called when the game starts or when spawned
void ACreatureSpawnCollider::BeginPlay()
{
	Super::BeginPlay();

	// Attach the on overlap delegate
	ColliderSphere->OnComponentBeginOverlap.AddDynamic(this, &ACreatureSpawnCollider::OnOverlapBegin);

}

/*
 * Overlap with spawner to spawn creatures
 */
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
		// Remove unneeded actors first
		TArray<AActor*> CreatureToRemove;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AberrantCreature, CreatureToRemove);

		// Process so long as there were actors to remove
		if (CreatureToRemove.Num() > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Destroy aberration creatures"));
			for (AActor* SingleSpawner : CreatureToRemove)
			{
				/*
				 * NOTE: There should be a better way to check whether to destroy the 
				 * actor "with no target"
				 */
				// Check if the creature has a target on the player
				AAbrrCreatureCharacterClass* Creature = Cast<AAbrrCreatureCharacterClass>(SingleSpawner);
				if (Creature)
				{
					if (Creature->PlayerTargetActor != nullptr)
					{
						Creature->Destroy();
					}
				}
			}
		}

		// Then spawn the new actors
		TArray<AActor*> CreatureSpawners;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), CreatureSpawnerBox, CreatureSpawners);

		// If there were actors found
		if (CreatureSpawners.Num() > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Spawn aberration creatures"));
			for (AActor* SingleSpawner : CreatureSpawners)
			{
				if (SingleSpawner->ActorHasTag(SpawnerTag))
				{
					AAbrrCreatureCharacterClass* Creature = GetWorld()->SpawnActor<AAbrrCreatureCharacterClass>(AberrantCreature, SingleSpawner->GetActorLocation(), FRotator(0.f, 0.f, 0.f));
					if (Creature)
					{
						if (FPSChar->bIsLookingThroughCamera) Creature->GetMesh()->SetMaterial(0, FPSChar->AbbrCreatureMat1);
						else Creature->GetMesh()->SetMaterial(0, FPSChar->AbbrCreatureMat2);
					}
				}
				// else do nothing
			}

			if (bShouldDestroy) Destroy();
		}
		// else do nothing
	}
}