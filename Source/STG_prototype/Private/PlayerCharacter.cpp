// Copyright Epic Games, Inc. All Rights Reserved.

#include "PlayerCharacter.h"
#include "PlayerShootingComponent.h"
#include "LockOnHandler.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LockOnHandler.h"
#include "Projectile.h"
#include "STGGameMode.h"
#include "Particles/ParticleSystem.h"
#include "Blueprint/UserWidget.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Particles/ParticleSystem.h"
#include "DrawDebugHelpers.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(GetCapsuleComponent());

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);

	LockOnSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Lock On Spawn Point"));
	LockOnSpawnPoint->SetupAttachment(RootComponent);

	TargetingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Targeting Mesh"));
	TargetingMesh->SetupAttachment(RootComponent);

	MainSocketComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Main Socket Component"));
	MainSocketComponent->SetupAttachment(RootComponent);

	NiagaraParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara System Component"));
	NiagaraParticles->SetupAttachment(MainSocketComponent);

	RibbonSocketComponentMiddle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara System Component1"));
	RibbonSocketComponentMiddle->SetupAttachment(BaseMesh);
	RibbonSocketComponentMiddle->bHiddenInGame = true;

	RibbonSocketComponentMidLeft = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara System Component2"));
	RibbonSocketComponentMidLeft->SetupAttachment(BaseMesh);
	RibbonSocketComponentMidLeft->bHiddenInGame = true;

	RibbonSocketComponentMidRight = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara System Component3"));
	RibbonSocketComponentMidRight->SetupAttachment(BaseMesh);
	RibbonSocketComponentMidRight->bHiddenInGame = true;

	RibbonSocketComponentLeft = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara System Component4"));
	RibbonSocketComponentLeft->SetupAttachment(BaseMesh);
	RibbonSocketComponentLeft->bHiddenInGame = true;

	RibbonSocketComponentRight = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara System Component5"));
	RibbonSocketComponentRight->SetupAttachment(BaseMesh);
	RibbonSocketComponentRight->bHiddenInGame = true;

	// Create shooting component
	ShootingComponent = CreateDefaultSubobject<UPlayerShootingComponent>(TEXT("Shooting Component"));
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerMappingContext, 0);
		}
	}

	GameMode = Cast<ASTGGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		int32 TestNum = GameMode->GetLives();
	}

	AddSocket();

	if (RibbonBluePrint)
	{
		AActor* NewActor = GetWorld()->SpawnActor<AActor>(RibbonBluePrint, FVector::ZeroVector, FRotator::ZeroRotator);
		if (NewActor)
		{
			NewActor->AttachToComponent(MainSocketComponent, FAttachmentTransformRules::KeepRelativeTransform);
		}
	}

	TargetingMesh->SetVisibility(false);
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ScanningForLockOn)
	{
		LockOn();
		RotateTargetingMesh();
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(Fire1Action, ETriggerEvent::Triggered, this, &APlayerCharacter::PrimaryShot);
		EnhancedInputComponent->BindAction(Fire1ReleaseAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Fire1ReleaseCharge);
		EnhancedInputComponent->BindAction(Fire2Action, ETriggerEvent::Triggered, this, &APlayerCharacter::SecondaryShot);
		EnhancedInputComponent->BindAction(LockOnAction, ETriggerEvent::Triggered, this, &APlayerCharacter::ScanForLockOn);
		EnhancedInputComponent->BindAction(LockOnReleaseAction, ETriggerEvent::Triggered, this, &APlayerCharacter::LockOnRelease);
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller)
	{
		FVector Forward = GetActorForwardVector();
		AddMovementInput(Forward, MovementVector.Y);

		FVector Right = GetActorRightVector();
		AddMovementInput(Right, MovementVector.X);
	}
}

void APlayerCharacter::PrimaryShot(const FInputActionValue& Value)
{
	PrimaryShotFiring = true;
	
	if (ShootingComponent)
	{
		ShootingComponent->StartPrimaryShot();
	}
}

void APlayerCharacter::SecondaryShot(const FInputActionValue& Value)
{
	if (ShootingComponent)
	{
		ShootingComponent->FireSecondaryShot();
	}
}

void APlayerCharacter::ScanForLockOn(const FInputActionValue& Value)
{
	TargetingMesh->SetVisibility(true);
	if (CanLockOn)
	{
		ScanningForLockOn = true;
		if (ShootingComponent)
		{
			ShootingComponent->StartLockOnScanning();
		}
	}
}

void APlayerCharacter::Fire1ReleaseCharge(const FInputActionValue& Value)
{
	if (ShootingComponent)
	{
		ShootingComponent->StartCharging();
	}
}

void APlayerCharacter::LockOnRelease(const FInputActionValue& Value)
{
	ScanningForLockOn = false;
	ParseLockOn();
	TargetingMesh->SetVisibility(false);
	
	if (ShootingComponent)
	{
		ShootingComponent->StopLockOnScanning();
		ShootingComponent->FireLockOnShots(ActorsLocked);
	}
}

void APlayerCharacter::PCTestFunc()
{
	// Test function implementation
}

void APlayerCharacter::HandleDestruction()
{
	if (DeathParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticles, GetActorLocation(), GetActorRotation());
	}

	if (DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	}

	if (DeathCameraShakeClass)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DeathCameraShakeClass);
	}

	GetMesh()->SetVisibility(false);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (GameMode)
	{
		GameMode->ActorDied(this);
	}

	SetLifeSpan(0.1f);
}

void APlayerCharacter::SetPowerLevel()
{
	if (ShootingComponent)
	{
		ShootingComponent->IncreaseFirePowerLevel();
	}
}

void APlayerCharacter::IncreasePowerLevel()
{
	if (ShootingComponent)
	{
		ShootingComponent->IncreaseFirePowerLevel();
	}
}

void APlayerCharacter::ResetShotDelayTimer()
{
	CanFire = true;
}

void APlayerCharacter::ResetLockOnDelayTimer()
{
	CanLockOn = true;
}

void APlayerCharacter::EndChargeOneTimer()
{
	// Charge level 1 reached
}

void APlayerCharacter::EndChargeTwoTimer()
{
	// Charge level 2 reached
}

void APlayerCharacter::EndChargeThreeTimer()
{
	// Charge level 3 reached
}

void APlayerCharacter::AddSocket()
{
	// Socket addition logic
}

void APlayerCharacter::LockOn()
{
	// Lock-on scanning logic
}

void APlayerCharacter::ParseLockOn()
{
	// Parse lock-on targets logic
}

bool APlayerCharacter::CheckCanRespawn()
{
	return true;
}

void APlayerCharacter::HandleRespawn()
{
	// Respawn logic
}

void APlayerCharacter::StartGhostPlayer()
{
	// Ghost player logic
}

void APlayerCharacter::StopGhostPlayer()
{
	// Stop ghost player logic
}

void APlayerCharacter::RotateTargetingMesh()
{
	if (TargetingMesh)
	{
		FRotator CurrentRotation = TargetingMesh->GetComponentRotation();
		CurrentRotation.Yaw += TargetingMeshRotationSpeed * GetWorld()->GetDeltaSeconds();
		TargetingMesh->SetWorldRotation(CurrentRotation);
	}
}
