// Fill out your copyright notice in the Description page of Project Settings.


#include "AbrrCreatureCharacterClass.h"
#include "BaseAIControllerClass.h"

#include "Components/SphereComponent.h"

#include "FPSPlayerCharacter.h"
#include "BaseCharacterClass.h"

// Sets default values
AAbrrCreatureCharacterClass::AAbrrCreatureCharacterClass()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*FleeRangeCollider = CreateDefaultSubobject<USphereComponent>("FleeRangeCollider");
	FleeRangeCollider->SetupAttachment(RootComponent);*/

	AgroRangeCollider = CreateDefaultSubobject<USphereComponent>("AgroRangeCollider");
	AgroRangeCollider->SetupAttachment(RootComponent);

	/*AttackRangeCollider = CreateDefaultSubobject<USphereComponent>("AttackRangeCollider");
	AttackRangeCollider->SetupAttachment(RootComponent);*/

	MaxHealth = 100.f;
	CurrentHealth = 100.f;

}

// Called when the game starts or when spawned
void AAbrrCreatureCharacterClass::BeginPlay()
{
	Super::BeginPlay();

	AgroRangeCollider->OnComponentBeginOverlap.AddDynamic(this, &AAbrrCreatureCharacterClass::OnOverlapBegin);
	AgroRangeCollider->OnComponentEndOverlap.AddDynamic(this, &AAbrrCreatureCharacterClass::OnOverlapEnd);

}

// Called every frame
void AAbrrCreatureCharacterClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Is dead, nothing to process
	if (bIsDead) return;

	if (GetVelocity().Z < 0)
	{
		// Fall damage
		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Creature vz:%f"), GetVelocity().Z));
		FallRes = GetVelocity().Z * (-1) - 1000;
		if (FallRes > 0)
		{
			bHasFallen = true;
			FallRes = FallRes / 10.f;
		}
	}
	else
	{
		if (bHasFallen)
		{
			bHasFallen = false;
			CurrentHealth -= FallRes;
			FallRes = 0.f;
		}
	}

	if (CurrentHealth < 0)
	{
		// Used for animation
		bHasDied = true;
		bIsDead = true;
		SetActorEnableCollision(false);
	}

	// Movement
	if (bHasPlayerTarget)
	{
		//GEngine->AddOnScreenDebugMessage(400, 3.f, FColor::Red, FString::Printf(TEXT("Player target")));
		MoveToTarget(PlayerTargetActor);
	}
	else
	{
		if (bHasBCharTarget)
		{
			//GEngine->AddOnScreenDebugMessage(404, 3.f, FColor::Red, FString::Printf(TEXT("BChar target")));
			
			// Move to character
			MoveToTarget(BCharTargetActor);
		}
		else
		{
			//GEngine->AddOnScreenDebugMessage(405, 3.f, FColor::Red, FString::Printf(TEXT("No Target")));

			// Move to random location
			if (TestTimer >= 4.f)
			{
				//GEngine->AddOnScreenDebugMessage(406, 3.f, FColor::Red, FString::Printf(TEXT("Move random")));
				RandomMove();
				TestTimer = 0.f;
			}
		}
	}

	TestTimer += DeltaTime;
	AttackTimer += DeltaTime;

	//// Random move
	//TestTimer += DeltaTime;
	//if (TestTimer >= 4.f)
	//{
	//	//GEngine->AddOnScreenDebugMessage(30, 3.f, FColor::Black, FString::Printf(TEXT("AbbrCreature: Moving!")));
	//	RandomMove();
	//	TestTimer = 0.f;
	//}
}

// Called to bind functionality to input
void AAbrrCreatureCharacterClass::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAbrrCreatureCharacterClass::RandomMove()
{
	ABaseAIControllerClass* AICon = Cast<ABaseAIControllerClass>(GetController());
	if (AICon)
	{
		//GEngine->AddOnScreenDebugMessage(31, 3.f, FColor::Black, FString::Printf(TEXT("AbbrCreature: Moving to random location!")));
		FVector ActorLocation = GetActorLocation();
		FVector NHeading = FVector(
			ActorLocation.X + (int)FMath::FRandRange(500.f, 5000.f) - 2750.f,
			ActorLocation.Y + (int)FMath::FRandRange(500.f, 5000.f) - 2750.f,
			ActorLocation.Z);
		AICon->MoveToLocation(NHeading);
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(31, 3.f, FColor::Red, FString::Printf(TEXT("AbbrCreature: No AI Controller found!")));
	}
}

void AAbrrCreatureCharacterClass::MoveToTarget(AActor* TargetActor)
{
	if (TargetActor)
	{
		if (TestTimer >= 0.5f)
		{
			if (TargetActor) AgroCharacter(TargetActor->GetActorLocation());
			TestTimer = 0.f;
		}

		// Distance difference to target
		FVector DDist = TargetActor->GetActorLocation() - GetActorLocation();
		float DistanceDiff = sqrtf(DDist.X * DDist.X
			+ DDist.Y * DDist.Y
			+ DDist.Z * DDist.Z);

		// Check the distance difference
		//GEngine->AddOnScreenDebugMessage(407, 3.f, FColor::Red, FString::Printf(TEXT("Distance: %f"), DistanceDiff));
		if (DistanceDiff < DrainRange)
		{
			//GEngine->AddOnScreenDebugMessage(401, 3.f, FColor::Red, FString::Printf(TEXT("In attack range")));
			bIsInAttackRange = true;
		}
		else
		{
			//GEngine->AddOnScreenDebugMessage(402, 3.f, FColor::Red, FString::Printf(TEXT("Not in attack range")));
			bIsInAttackRange = false;
		}

		if (bIsInAttackRange)
		{
			//GEngine->AddOnScreenDebugMessage(403, 3.f, FColor::Red, FString::Printf(TEXT("Can attack!")));
			if (AttackTimer >= 0.5f)
			{
				// FPSCharacter
				AFPSPlayerCharacter* FPSChar = Cast<AFPSPlayerCharacter>(TargetActor);
				if (FPSChar)
				{
					//GEngine->AddOnScreenDebugMessage(511, 3.f, FColor::Red, FString::Printf(TEXT("Draining sanity!")));
					FPSChar->PlayerSanity -= DrainPower;
				}

				// BaseCharacter
				ABaseCharacterClass* BChar = Cast<ABaseCharacterClass>(TargetActor);
				if (BChar)
				{
					//GEngine->AddOnScreenDebugMessage(511, 3.f, FColor::Red, FString::Printf(TEXT("Draining sanity!")));
					BChar->CurrentSanity -= 40.f;
				}

				AttackTimer = 0.f;
			}
		}
	}
}

// Overlap for fleeing
void AAbrrCreatureCharacterClass::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	// Is dead don't process
	if (bIsDead) return;

	// Check distance to player
	AFPSPlayerCharacter* FPSChar = Cast<AFPSPlayerCharacter>(OtherActor);
	if (FPSChar)
	{
		/*FVector PlayerLocation = FPSChar->GetActorLocation();
		FVector MyLocation = GetActorLocation();

		FVector DistanceResult = PlayerLocation - MyLocation;
		float Distance = sqrt(DistanceResult.X * DistanceResult.X + DistanceResult.Y * DistanceResult.Y + DistanceResult.Z * DistanceResult.Z);
		Distance = abs(Distance);

		GEngine->AddOnScreenDebugMessage(31, 3.f, FColor::Red, FString::Printf(TEXT("Distance to player: %f"), Distance));*/

		//HeadingLocation = FPSChar->GetActorLocation();
		PlayerTargetActor = OtherActor;
		PlayerTargetLocation = FPSChar->GetActorLocation();
		bHasPlayerTarget = true;
	}
	else
	{
		// Was not the player character
		//GEngine->AddOnScreenDebugMessage(31, 3.f, FColor::Red, FString::Printf(TEXT("Not player")));

		// Was the base character
		ABaseCharacterClass* BaseChar = Cast<ABaseCharacterClass>(OtherActor);
		if (BaseChar)
		{
			//HeadingLocation = BaseChar->GetActorLocation();
			BCharTargetActor = OtherActor;
			BCharTargetLocation = BaseChar->GetActorLocation();
			bHasBCharTarget = true;
		}
		else
		{
			// Was neither the player or the base character
			// Do nothing
		}
	}
}

void AAbrrCreatureCharacterClass::OnOverlapEnd(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	// Is dead don't process
	if (bIsDead) return;

	AFPSPlayerCharacter* FPSChar = Cast<AFPSPlayerCharacter>(OtherActor);
	if (FPSChar)
	{
		bHasPlayerTarget = false;
		PlayerTargetActor = nullptr;
	}
	else
	{
		ABaseCharacterClass* BaseChar = Cast<ABaseCharacterClass>(OtherActor);
		if (BaseChar)
		{
			bHasBCharTarget = false;
			BCharTargetActor = nullptr;
		}
		else
		{
			// Was neither the player or the base character
			// Do nothing
		}
	}
}

// Overlap for fleeing
void AAbrrCreatureCharacterClass::FleeFromCharacter()
{
	// Is dead don't process
	if (bIsDead) return;

}

// Overlap for agro
void AAbrrCreatureCharacterClass::AgroCharacter(FVector NHeading)
{
	// Is dead don't process
	if (bIsDead) return;

	ABaseAIControllerClass* AICon = Cast<ABaseAIControllerClass>(GetController());
	if (AICon)
	{
		AICon->MoveToLocation(NHeading);
	}
}

// Overlap for attack
void AAbrrCreatureCharacterClass::AttackCharacter()
{
	// Is dead don't process
	if (bIsDead) return;

}