// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseStructureClass.h"
//#include "GameFramework/Actor.h"
#include "CityBuilding01.generated.h"

UCLASS()
class LENSCHROMA_API ACityBuilding01 : public ABaseStructureClass
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACityBuilding01();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
