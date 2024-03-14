// Fill out your copyright notice in the Description page of Project Settings.


#include "BoostUpwardClass.h"
#include "Components/SphereComponent.h"

#include "FPSPlayerCharacter.h"

// Sets default values
ABoostUpwardClass::ABoostUpwardClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ColliderSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ColliderSphere"));
	SetRootComponent(ColliderSphere);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(GetRootComponent());

	BoostStrength = 1500.f;

}

// Called when the game starts or when spawned
void ABoostUpwardClass::BeginPlay()
{
	Super::BeginPlay();

	ColliderSphere->OnComponentBeginOverlap.AddDynamic(this, &ABoostUpwardClass::OnOverlapBegin);

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	
}

// Called every frame
void ABoostUpwardClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

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
		// Launch the character
		FVector FPSDirection = FVector(
			FPSChar->GetVelocity().X, 
			FPSChar->GetVelocity().Y, 
			FPSChar->GetVelocity().Z + BoostStrength);

		//FPSChar->LaunchCharacter(FPSDirection * 100, true, true);
		FPSChar->LaunchCharacter(FPSDirection, true, true);
	}
	/*else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Not the Character!")));
	}*/
}