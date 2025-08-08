// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Actor.h" // needed?
#include "Components/SceneComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/ActorComponent.h"
#include "Components/SplineComponent.h"
#include "SplineMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STG_PROTOTYPE_API USplineMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USplineMovementComponent();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USplineComponent* SplineComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UFloatingPawnMovement* PawnMovement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spline")
	float MovementSpeed;

	float CurrentDistance;

	int32 CurrentSplinePointIndex;

public:	
	UFUNCTION(BlueprintCallable, Category = "Spline")
	void MoveAlongSpline(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "Spline")
	void SetSplineComponent(USplineComponent* NewSplineComponent);

	UFUNCTION(BlueprintCallable, Category = "Spline")
	void SetMovementSpeed(float NewMovementSpeed);

	UFUNCTION(BlueprintCallable, Category = "Spline")
	FVector GetSplineStartLocation();
};
