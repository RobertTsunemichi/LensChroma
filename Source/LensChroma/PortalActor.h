// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableInterface.h"
#include "BaseStructureClass.h"
#include "PortalActor.generated.h"

UCLASS()
class LENSCHROMA_API APortalActor : public ABaseStructureClass, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortalActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* ColliderSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TeleTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName LevelName;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void ActivateInteraction_Implementation(AActor* InActor, int InInt) override;
};
