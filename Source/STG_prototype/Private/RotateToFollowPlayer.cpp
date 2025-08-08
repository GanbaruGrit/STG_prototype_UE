
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerController.h"
#include "RotateToFollowPlayer.h"

// Sets default values for this component's properties
URotateToFollowPlayer::URotateToFollowPlayer()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	/*RotatingSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Rotating Scene Component"));*/
	
	/*if (GetOwner())
	{
		RotatingSceneComponent->SetupAttachment(GetOwner()->GetRootComponent());
	}*/

	/*RotatingMesh = CreateDefaultSubobject<UMeshComponent>(TEXT("Rotating Mesh"));*/
	
	/*if (GetOwner())
	{
		RotatingMesh->SetupAttachment(RotatingSceneComponent);
	}*/
}


// Called when the game starts
void URotateToFollowPlayer::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void URotateToFollowPlayer::RotateSceneComponent()
{
	// Get the player controller
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		// Get the player's location
		FVector PlayerLocation = PlayerController->GetPawn()->GetActorLocation();

		// Calculate the rotation needed for the scene component to look at the player
		FVector DirectionToPlayer = PlayerLocation - this->GetComponentLocation();
		FRotator NewRotation = FRotator(0.f, DirectionToPlayer.Rotation().Yaw, 0.f);

		// Set the rotation of the scene component
		this->SetWorldRotation(NewRotation);
		//this->SetRelativeRotation(NewRotation);

		
	}
}


// Called every frame
void URotateToFollowPlayer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RotateSceneComponent();
}

