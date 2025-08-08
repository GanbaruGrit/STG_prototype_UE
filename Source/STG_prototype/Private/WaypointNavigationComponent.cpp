// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "WaypointNavigationComponent.h"

// Sets default values for this component's properties
UWaypointNavigationComponent::UWaypointNavigationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UWaypointNavigationComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerActor = GetOwner();
    OwnerLocation = OwnerActor->GetActorLocation();
	
    //GetWorld()->SpawnActor<AActor>(Test, FVector::ZeroVector, FRotator::ZeroRotator);;

    //WaypointsArray.Add()
}


// Called every frame
void UWaypointNavigationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    CachedDeltaTime = DeltaTime;

    MoveToWaypoint();
}

void UWaypointNavigationComponent::MoveToWaypoint()
{
    if (WaypointsArray.Num() == 0 || OwnerActor == nullptr)
    {
        return;
    }

    // Refresh owner location each tick
    OwnerLocation = OwnerActor->GetActorLocation();

    // Get the current waypoint location
    const FVector TargetLocation = WaypointsArray[CurrentWaypointIndex]->GetActorLocation();

    // Calculate the direction to the waypoint
    FVector Direction = (TargetLocation - OwnerLocation);
    Direction.Normalize();

    // Move towards the waypoint in world space
    OwnerActor->AddActorWorldOffset(Direction * MovementSpeed * CachedDeltaTime, true);

    // Check if the pawn is close to the waypoint
    if (FVector::DistSquared(OwnerActor->GetActorLocation(), TargetLocation) < FMath::Square(50.0f))
    {
        // Move to the next waypoint
        CurrentWaypointIndex = (CurrentWaypointIndex + 1) % WaypointsArray.Num();
    }
}

