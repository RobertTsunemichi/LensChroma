// Fill out your copyright notice in the Description page of Project Settings.


#include "FinalStageSetcollider.h"
#include "FPSPlayerCharacter.h"
#include "Components/SphereComponent.h"

// Sets default values
AFinalStageSetcollider::AFinalStageSetcollider()
{
	ColliderSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ColliderSphere"));
	ColliderSphere->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AFinalStageSetcollider::BeginPlay()
{
	Super::BeginPlay();

	ColliderSphere->OnComponentBeginOverlap.AddDynamic(this, &AFinalStageSetcollider::OnOverlapBegin);
	
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
	AFPSPlayerCharacter* FPSChar = Cast<AFPSPlayerCharacter>(OtherActor);
	if (FPSChar)
	{
		FPSChar->bFinalStage = true;
		Destroy();
	}
}