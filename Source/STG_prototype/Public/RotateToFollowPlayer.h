// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "RotateToFollowPlayer.generated.h"

class USceneComponent;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STG_PROTOTYPE_API URotateToFollowPlayer : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URotateToFollowPlayer();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
    UPROPERTY()
    USceneComponent* RotatingSceneComponent = nullptr;
	//UMeshComponent* RotatingMesh;

	void RotateSceneComponent();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
