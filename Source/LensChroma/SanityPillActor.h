// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/Actor.h"
#include "BaseItemClass.h"
#include "SanityPillActor.generated.h"

UCLASS()
class LENSCHROMA_API ASanityPillActor : public ABaseItemClass
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASanityPillActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RecoveryAmount = 100.f;

};
