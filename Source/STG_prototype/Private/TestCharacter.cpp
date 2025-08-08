// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TestCharacter.h"
#include "ReactiveSphere.h"

// Sets default values
ATestCharacter::ATestCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerMappingContext, 0);
		}

	}
}


// Called every frame
void ATestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATestCharacter::Move);
		
	}
}

void ATestCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	/*UE_LOG(LogTemp, Warning, TEXT("In Move. MovementVector %s"), *MovementVector.ToString());
	UE_LOG(LogTemp, Warning, TEXT("In Move. Controller? %s"), Controller ? "True" : "False");*/

	if (Controller)
	{
		FVector Forward = GetActorForwardVector();

		//UE_LOG(LogTemp, Warning, TEXT("In Move. Forward %s"), *Forward.ToString());
		AddMovementInput(Forward, MovementVector.Y);
		UCharacterMovementComponent* MovementComp;
		MovementComp = GetCharacterMovement();
		//MovementComp.PerformMo

		FVector Right = GetActorRightVector();
		//UE_LOG(LogTemp, Warning, TEXT("In Move. Right %s"), *Right.ToString());
		AddMovementInput(Right, MovementVector.X);
	}
}
