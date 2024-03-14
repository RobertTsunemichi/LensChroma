// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraLens.h"
#include "Components/SceneCaptureComponent2D.h"

// Sets default values
ACameraLens::ACameraLens()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CaptureCamera2D = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("CaptureCamera2D"));
	CaptureCamera2D->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ACameraLens::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACameraLens::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

