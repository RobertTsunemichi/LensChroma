// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacterClass.h"
#include "BaseAIControllerClass.h"

// Sets default values
ABaseCharacterClass::ABaseCharacterClass()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MaxSanity = 100.f;
	CurrentSanity = 100.f;
}

// Called when the game starts or when spawned
void ABaseCharacterClass::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseCharacterClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Is dead, nothing to process
	if (bIsDead) return;

	if (GetVelocity().Z < 0)
	{
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
			CurrentSanity -= FallRes;
			FallRes = 0.f;
		}
	}

	if (CurrentSanity < 0)
	{
		// Used for animation
		bHasDied = true;
		bIsDead = true;
		SetActorEnableCollision(false);
	}

	TestTimer += DeltaTime;

	if (!bIsFleeing)
	{
		if (TestTimer >= 4.f)
		{
			//GEngine->AddOnScreenDebugMessage(30, 3.f, FColor::Black, FString::Printf(TEXT("BCC: Moving!")));
			RandomMove();
			TestTimer = 0.f;
		}
	}
}

// Called to bind functionality to input
void ABaseCharacterClass::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseCharacterClass::RandomMove()
{
	ABaseAIControllerClass* AICon = Cast<ABaseAIControllerClass>(GetController());
	if (AICon)
	{
		//GEngine->AddOnScreenDebugMessage(31, 3.f, FColor::Black, FString::Printf(TEXT("BCC: Moving to random location!")));
		FVector ActorLocation = GetActorLocation();
		FVector NHeading = FVector(
			ActorLocation.X + (int)FMath::FRandRange(500.f, 5000.f) - 2750.f,
			ActorLocation.Y + (int)FMath::FRandRange(500.f, 5000.f) - 2750.f,
			ActorLocation.Z);
		AICon->MoveToLocation(NHeading);
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(31, 3.f, FColor::Red, FString::Printf(TEXT("BCC: No AI Controller found!")));
	}
}

void ABaseCharacterClass::FleeMove(FVector InVector)
{

	ABaseAIControllerClass* AICon = Cast<ABaseAIControllerClass>(GetController());
	if (AICon)
	{
		AICon->MoveToLocation(InVector);
	}
}