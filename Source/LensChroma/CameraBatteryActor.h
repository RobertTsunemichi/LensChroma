// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseItemClass.h"
#include "CameraBatteryActor.generated.h"

UCLASS()
class LENSCHROMA_API ACameraBatteryActor : public ABaseItemClass
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACameraBatteryActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ChargeAmount = 100.f;

};
