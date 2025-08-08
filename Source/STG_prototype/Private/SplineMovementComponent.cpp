// Fill out your copyright notice in the Description page of Project Settings.


#include "SplineMovementComponent.h"

// Sets default values for this component's properties
USplineMovementComponent::USplineMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	// Create components
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Component From SMC"));

	PawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Pawn Movement"));

	// Set default values
	MovementSpeed = 100.0f;
    CurrentDistance = 0.0f;

    /*if (GetOwner())
    {
        SplineComponent->SetupAttachment(GetOwner()->GetRootComponent());
    }*/
}

void USplineMovementComponent::MoveAlongSpline(float DeltaTime)
{
    if (SplineComponent)
    {
        // Update the current distance based on speed and time
        CurrentDistance += MovementSpeed * DeltaTime;

        // Get the location and rotation from the spline at the current distance
        FVector Location = SplineComponent->GetLocationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::Local);
        FRotator Rotation = SplineComponent->GetRotationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::Local);

        // Set the actor's location and rotation
        GetOwner()->SetActorLocationAndRotation(Location, Rotation);



        //FVector LocalSplineLocation = SplineComponent->GetLocationAtSplinePoint(CurrentSplinePointIndex, ESplineCoordinateSpace::Local);
        //UE_LOG(LogTemp, Warning, TEXT("LocalSplineLocation: %s"), *LocalSplineLocation.ToString());

        //// Move the owning actor towards the local spline location
        //AActor* OwningActor = GetOwner();
        //if (OwningActor)
        //{
        //    FVector WorldSplineLocation = OwningActor->GetTransform().TransformPosition(LocalSplineLocation);
        //    UE_LOG(LogTemp, Warning, TEXT("WorldSplineLocation: %s"), *WorldSplineLocation.ToString());

        //    FVector Direction = WorldSplineLocation - OwningActor->GetActorLocation();
        //    Direction.Normalize();
        //    OwningActor->AddActorWorldOffset(Direction * MovementSpeed * GetWorld()->GetDeltaSeconds());

        //    // Check if the actor is close enough to the current spline point
        //    if (FVector::DistSquared(OwningActor->GetActorLocation(), WorldSplineLocation) < FMath::Square(50.0f))
        //    {
        //        if (CurrentSplinePointIndex == (SplineComponent->GetNumberOfSplinePoints() - 1))
        //            {
        //                return;
        //            }

        //        // Move to the next spline point
        //        CurrentSplinePointIndex = (CurrentSplinePointIndex + 1) % SplineComponent->GetNumberOfSplinePoints();
        //    }
        //}

        // For moving across World spline location
        
        //FVector SplineLocation = SplineComponent->GetLocationAtSplinePoint(CurrentSplinePointIndex, ESplineCoordinateSpace::World);

        //// Move the owning actor towards the spline location
        //AActor* OwningActor = GetOwner();
        //if (OwningActor)
        //{
        //    FVector Direction = SplineLocation - OwningActor->GetActorLocation();
        //    Direction.Normalize();
        //    OwningActor->AddActorWorldOffset(Direction * MovementSpeed * GetWorld()->GetDeltaSeconds());

        //    // Check if the actor is close enough to the current spline point
        //    if (FVector::DistSquared(OwningActor->GetActorLocation(), SplineLocation) < FMath::Square(50.0f))
        //    {
        //        if (CurrentSplinePointIndex == (SplineComponent->GetNumberOfSplinePoints() - 1))
        //        {
        //            return;
        //        }
        //        
        //        // Move to the next spline point
        //        CurrentSplinePointIndex = (CurrentSplinePointIndex + 1) % SplineComponent->GetNumberOfSplinePoints();
        //    }
        //}
    }
}

void USplineMovementComponent::SetSplineComponent(USplineComponent* NewSplineComponent)
{
    SplineComponent = NewSplineComponent;
    CurrentSplinePointIndex = 0;
}

void USplineMovementComponent::SetMovementSpeed(float NewMovementSpeed)
{
    MovementSpeed = NewMovementSpeed;
}

FVector USplineMovementComponent::GetSplineStartLocation()
{
    return SplineComponent->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World);
}

