// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WaypointNavigationComponent.generated.h"

class AActor;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STG_PROTOTYPE_API UWaypointNavigationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWaypointNavigationComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY(EditAnywhere, Category = "Waypoints")
    TArray<AActor*> WaypointsArray;
private:
    UPROPERTY(EditAnywhere, Category = "Waypoints")
    int CurrentWaypointIndex = 0;

	UPROPERTY(EditAnywhere, Category = "Waypoints")
    float MovementSpeed = 300.f;

	AActor* OwnerActor;
	FVector OwnerLocation;
	float CachedDeltaTime;

	UFUNCTION(BlueprintCallable, Category = "Waypoints")
	void MoveToWaypoint();
		
};
