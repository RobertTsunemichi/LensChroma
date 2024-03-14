// Fill out your copyright notice in the Description page of Project Settings.


#include "FinalStageSetcollider.h"
#include "FPSPlayerCharacter.h"
#include "Components/SphereComponent.h"

// Sets default values
AFinalStageSetcollider::AFinalStageSetcollider()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ColliderSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ColliderSphere"));
	ColliderSphere->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AFinalStageSetcollider::BeginPlay()
{
	Super::BeginPlay();

	ColliderSphere->OnComponentBeginOverlap.AddDynamic(this, &AFinalStageSetcollider::OnOverlapBegin);
	
}

// Called every frame
void AFinalStageSetcollider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Overlap for fleeing
void AFinalStageSetcollider::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT(">>> Set final stage")));
	// FPSCharacter
	AFPSPlayerCharacter* FPSChar = Cast<AFPSPlayerCharacter>(OtherActor);
	if (FPSChar)
	{
		FPSChar->bFinalStage = true;
		Destroy();
	}
}