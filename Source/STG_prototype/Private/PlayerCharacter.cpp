// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
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
#include "GameFramework/FloatingPawnMovement.h" // prob not needed
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
		int TestNum = GameMode->GetLives();
		//UE_LOG(LogTemp, Warning, TEXT("Game Mode Lives: %d"), TestNum);
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

// Called to bind functionality to input
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
	/*UE_LOG(LogTemp, Warning, TEXT("In Move. MovementVector %s"), *MovementVector.ToString());
	UE_LOG(LogTemp, Warning, TEXT("In Move. Controller? %s"), Controller ? "True" : "False");*/

	if (Controller)
	{
		FVector Forward = GetActorForwardVector();

		//UE_LOG(LogTemp, Warning, TEXT("In Move. Forward %s"), *Forward.ToString());
		AddMovementInput(Forward, MovementVector.Y);

		FVector Right = GetActorRightVector();
		//UE_LOG(LogTemp, Warning, TEXT("In Move. Right %s"), *Right.ToString());
		AddMovementInput(Right, MovementVector.X);
	}
}

void APlayerCharacter::PrimaryShot(const FInputActionValue& Value)
{
	PrimaryShotFiring = true;

	if (CurrentChargeType == EChargePowerLevel::ECPL_2)
	{
		FVector Location = ProjectileSpawnPoint->GetComponentLocation();
		FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();
		AProjectile* ChargeShotOne = CreateProjectile(ChargeOneProjectileClass, Location, Rotation);
		ChargeShotOne->Tags.Add(FName("Charge Shot"));
		CurrentChargeType = EChargePowerLevel::ECPL_1;
	}

	if (CurrentChargeType == EChargePowerLevel::ECPL_3)
	{
		FVector Location = ProjectileSpawnPoint->GetComponentLocation();
		FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();
		AProjectile* ChargeShotTwo = CreateProjectile(ChargeTwoProjectileClass, Location, Rotation);
		ChargeShotTwo->Tags.Add(FName("Charge Shot"));
		CurrentChargeType = EChargePowerLevel::ECPL_1;
	}

	if (CurrentChargeType == EChargePowerLevel::ECPL_4)
	{
		FVector Location = ProjectileSpawnPoint->GetComponentLocation();
		FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();
		AProjectile* ChargeShotThree = CreateProjectile(ChargeThreeProjectileClass, Location, Rotation);
		ChargeShotThree->Tags.Add(FName("Charge Shot"));
		CurrentChargeType = EChargePowerLevel::ECPL_1;
	}

	GetWorld()->GetTimerManager().ClearTimer(ChargeOneTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(ChargeTwoTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(ChargeThreeTimerHandle);

	//UE_LOG(LogTemp, Warning, TEXT("Proj Spawn Point in Fire %s"), *Location.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("PrimaryShotFiring Pre: %s"), (PrimaryShotFiring ? TEXT("True") : TEXT("False")));

	if (CanFire)
	{

		if (CurrentFireType == EFirePowerLevel::EFPL_1)
		{
			
			ShotPower_1();
			UGameplayStatics::PlaySoundAtLocation(this, PrimaryFireSound, GetActorLocation());
		}

		if (CurrentFireType == EFirePowerLevel::EFPL_2)
		{
			ShotPower_2();
			UGameplayStatics::PlaySoundAtLocation(this, PrimaryFireSound, GetActorLocation());
		}

		if (CurrentFireType == EFirePowerLevel::EFPL_3)
		{
			ShotPower_3();
			UGameplayStatics::PlaySoundAtLocation(this, PrimaryFireSound, GetActorLocation());
		}

		if (CurrentFireType == EFirePowerLevel::EFPL_4)
		{
			ShotPower_4();
			UGameplayStatics::PlaySoundAtLocation(this, PrimaryFireSound, GetActorLocation());
		}

		if (CurrentFireType == EFirePowerLevel::EFPL_5)
		{
			ShotPower_5();
			UGameplayStatics::PlaySoundAtLocation(this, PrimaryFireSound, GetActorLocation());
		}

	}

	GetWorld()->GetTimerManager().SetTimer(ShotDelayTimerHandle, this, &APlayerCharacter::ResetShotDelayTimer, ShotDelay, false);

	CanFire = false;

	PrimaryShotFiring = false;
}

void APlayerCharacter::SecondaryShot(const FInputActionValue& Value)
{
	if (CanFire)
	{
		FVector Location = ProjectileSpawnPoint->GetComponentLocation();
		FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();
		CreateProjectile(SubProjectileClass, Location, Rotation);

		FRotator Rotation2 = Rotation + FRotator(0.f, 15.f, 0.f);
		FVector Location2 = Location + FVector(0.f, 10.f, 0.f);
		CreateProjectile(SubProjectileClass, Location, Rotation2);

		FRotator Rotation3 = Rotation + FRotator(0.f, -15.f, 0.f);
		FVector Location3 = Location + FVector(0.f, -10.f, 0.f);
		CreateProjectile(SubProjectileClass, Location, Rotation3);

		UGameplayStatics::PlaySoundAtLocation(this, SecondaryFireSound, GetActorLocation());
	}

	GetWorld()->GetTimerManager().SetTimer(ShotDelayTimerHandle, this, &APlayerCharacter::ResetShotDelayTimer, ShotDelay, false);

	CanFire = false;

	//UE_LOG(LogTemp, Warning, TEXT("2nd Fire!"));
}

void APlayerCharacter::ScanForLockOn(const FInputActionValue& Value)
{
	TargetingMesh->SetVisibility(true);
	if (CanLockOn)
	{
		ScanningForLockOn = true;
	}
}

void APlayerCharacter::Fire1ReleaseCharge(const FInputActionValue& Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Fire1Release!"));

	GetWorld()->GetTimerManager().SetTimer(ChargeOneTimerHandle, this, &APlayerCharacter::EndChargeOneTimer, .8f, false);
	GetWorld()->GetTimerManager().SetTimer(ChargeTwoTimerHandle, this, &APlayerCharacter::EndChargeTwoTimer, 1.4f, false);
	GetWorld()->GetTimerManager().SetTimer(ChargeThreeTimerHandle, this, &APlayerCharacter::EndChargeThreeTimer, 2.2f, false);

	//(ShotDelayTimerHandle, this, &ABasePlayer::ResetShotDelayTimer, ShotDelay, false);
}

void APlayerCharacter::LockOnRelease(const FInputActionValue& Value)
{
	
	if (GEngine)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Lock On Release Trigger")));
	}

	ScanningForLockOn = false;
	ParseLockOn();
	TargetingMesh->SetVisibility(false);
}

void APlayerCharacter::PCTestFunc()
{
}

EFirePowerLevel APlayerCharacter::GetPowerLevel()
{
	return CurrentFireType;
}

void APlayerCharacter::SetPowerLevel()
{
	if (CurrentPowerLevel < 5)
	{
		CurrentPowerLevel++;
	}

	switch (CurrentPowerLevel)
	{
	case 1:
		CurrentFireType = EFirePowerLevel::EFPL_1;
			break;
	case 2:
		CurrentFireType = EFirePowerLevel::EFPL_2;
			break;
	case 3:
		CurrentFireType = EFirePowerLevel::EFPL_3;
		RibbonSocketComponentMiddle->bHiddenInGame = false;
			break;
	case 4:
		CurrentFireType = EFirePowerLevel::EFPL_4;
		RibbonSocketComponentMidLeft->bHiddenInGame = false;
		RibbonSocketComponentMidRight->bHiddenInGame = false;
			break;
	case 5:
		CurrentFireType = EFirePowerLevel::EFPL_5;
		RibbonSocketComponentLeft->bHiddenInGame = false;
		RibbonSocketComponentRight->bHiddenInGame = false;
			break;
	}
}

EChargePowerLevel APlayerCharacter::GetChargeLevel()
{
	return CurrentChargeType;
	/*UE_LOG(LogTemp, Warning, TEXT("Current Charge Level %d"), CurrentChargeType);
	return CurrentChargeType;*/
}

void APlayerCharacter::SetChargeLevel(EChargePowerLevel NewChargeLevel)
{
	CurrentChargeType = NewChargeLevel;

	const FString Message = UEnum::GetValueAsString(CurrentChargeType);

	//UE_LOG(LogTemp, Warning, TEXT("Charge type changed to: %s"), *Message);

	/*switch (CurrentChargeLevel)
	{
	case 1:
		CurrentChargeType = EChargePowerLevel::ECPL_1;
		break;
	case 2:
		CurrentChargeType = EChargePowerLevel::ECPL_2;
		UE_LOG(LogTemp, Warning, TEXT("Charge type changed to 2"));
		break;
	case 3:
		CurrentChargeType = EChargePowerLevel::ECPL_3;
		UE_LOG(LogTemp, Warning, TEXT("Charge type changed to 3"));
		break;
	case 4:
		CurrentChargeType = EChargePowerLevel::ECPL_4;
		UE_LOG(LogTemp, Warning, TEXT("Charge type changed to 4"));
		break;
	}*/
}

void APlayerCharacter::HandleDestruction()
{
	UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter HandleDestruction Hit"));
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	GameMode->ReduceLife();
	HandleRespawn();
	
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
}

void APlayerCharacter::ResetShotDelayTimer()
{
	CanFire = true;
}

void APlayerCharacter::ResetLockOnDelayTimer()
{
	ParseLockOn();

	CanLockOn = true;
}

void APlayerCharacter::EndChargeOneTimer()
{
	//UE_LOG(LogTemp, Warning, TEXT("ONE!"));
	SetChargeLevel(EChargePowerLevel::ECPL_2);
	FVector Location = ProjectileSpawnPoint->GetComponentLocation();
	FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();
	//CreateProjectile(ChargeOneProjectileClass, Location, Rotation);
}

void APlayerCharacter::EndChargeTwoTimer()
{
	//UE_LOG(LogTemp, Warning, TEXT("TWO!"));
	SetChargeLevel(EChargePowerLevel::ECPL_3);
	FVector Location = ProjectileSpawnPoint->GetComponentLocation();
	FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();
	//CreateProjectile(ChargeTwoProjectileClass, Location, Rotation);
}

void APlayerCharacter::EndChargeThreeTimer()
{
	//UE_LOG(LogTemp, Warning, TEXT("THREE!"));
	SetChargeLevel(EChargePowerLevel::ECPL_4);
	FVector Location = ProjectileSpawnPoint->GetComponentLocation();
	FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();
	//CreateProjectile(ChargeThreeProjectileClass, Location, Rotation);
}

void APlayerCharacter::AddSocket()
{
	USceneComponent* SocketComponent = NewObject<USceneComponent>(this, TEXT("Added Socket"));
	if (SocketComponent)
	{
		SocketComponent->SetRelativeLocation(FVector(-100.f, 0.f, 0.f));
		SocketComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		//if (RibbonActor)
		//{
		//	RibbonActor->AttachToComponent(SocketComponent, FAttachmentTransformRules::KeepRelativeTransform);
		//	//SocketComponent->AttachToComponent(RibbonMesh, FAttachmentTransformRules::KeepRelativeTransform);
		//}
	}
}

void APlayerCharacter::LockOn()
{
	//UKismetSystemLibrary::BoxTraceMulti(GetWorld(), Start, End, BoxHalfSize, BoxRotation, TraceChannel, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, OutHits, true);

	FVector Start = GetActorLocation();
	/*FVector PlayerStart = GetActorLocation();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Hit Result: %s"), *PlayerStart.ToString()));*/
	FVector End = Start += LockOnBoxEndRange;
	FRotator BoxRotation = FRotator(0.f, 0.f, 0.f);
	ETraceTypeQuery TraceChannel = ETraceTypeQuery::TraceTypeQuery2;
	TArray<FHitResult> OutHits;
	TArray<AActor*> ActorsToIgnore;

	bool isHit = UKismetSystemLibrary::BoxTraceMulti(GetWorld(), Start, End, LockOnBoxHalfSize, BoxRotation, TraceChannel, false, ActorsToIgnore, EDrawDebugTrace::None, OutHits, true);


	if (isHit)
	{

		for (auto& Hit : OutHits)
		{
			AActor* HitActor = Hit.GetActor();


			if (HitActor->ActorHasTag("Lockable"))
			{
				Hit.GetComponent()->SetMaterial(0, LockOnMaterial);

				ActorsLocked.AddUnique(HitActor);
				AActor* TempActor = ActorsLocked.Last();

				if (GEngine)
				{
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Hit Result: %s"), TempActor));
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("All Hit Information: %s"), *Hit.ToString()));
				}

				//UE_LOG(LogTemp, Warning, TEXT("ActorsLocked: "), ActorsLocked[0];

				/*AProjectile* LockOnProjectile = CreateLockOnProjectile(LockOnSpawnPoint->GetComponentLocation(), LockOnSpawnPoint->GetComponentRotation());
				UProjectileMovementComponent* LockOnProjectileMovement = Cast<UProjectileMovementComponent>(LockOnProjectile->GetComponentByClass(UProjectileMovementComponent::StaticClass()));

				LockOnProjectileMovement->HomingTargetComponent = HitActor->GetRootComponent();*/

				//CanLockOn = false;
				//GetWorld()->GetTimerManager().SetTimer(LockOnDelayTimerHandle, this, &ABasePlayer::ResetLockOnDelayTimer, LockOnDelay, false);
			}


			/*GetWorld()->SpawnActor<UStaticMesh>(
				TargetingMesh,
				Location,
				Rotation
			);
			Projectile->SetOwner(this);*/

		}
	}

	/*CanLockOn = false;
		GetWorld()->GetTimerManager().SetTimer(LockOnDelayTimerHandle, this, &ABasePlayer::ResetLockOnDelayTimer, LockOnDelay, false);*/


}

void APlayerCharacter::ParseLockOn()
{
	//UE_LOG(LogTemp, Warning, TEXT("LockOn Array Index: %d"), i);

	

	if (i < ActorsLocked.Num())
	{
		FString ActorName = ActorsLocked[i]->GetName();
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("LockOn Array Result: %s"), ActorName));
		UE_LOG(LogTemp, Warning, TEXT("LockOn Array Result: %s"), *ActorName);
		if (ActorsLocked[i]->ActorHasTag("Lockable"))

			if (GEngine)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("LockOn Array Result: %s"), ActorsLocked[i]));
				// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("All Hit Information: %s"), *Hit.ToString()));
			}

		{
			AProjectile* LockOnProjectile = CreateLockOnProjectile(LockOnSpawnPoint->GetComponentLocation(), LockOnSpawnPoint->GetComponentRotation());
			UProjectileMovementComponent* LockOnProjectileMovement = Cast<UProjectileMovementComponent>(LockOnProjectile->GetComponentByClass(UProjectileMovementComponent::StaticClass()));

			//LockOnProjectileMovement->HomingTargetComponent = ActorsLocked[i]->GetRootComponent();
			LockOnProjectileMovement->HomingTargetComponent = ActorsLocked[i]->FindComponentByClass<UStaticMeshComponent>();
			
			
			//UE_LOG(LogTemp, Warning, TEXT("LockOn Array Index: %d"), i);
			if (LockOnSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, LockOnSound, GetActorLocation());
			}

			i++;
			GetWorld()->GetTimerManager().SetTimer(LockOnDelayTimerHandle, this, &APlayerCharacter::ResetLockOnDelayTimer, LockOnDelay, false);
		}

	}
	else
	{
		ActorsLocked.Empty();
		i = 0;
	}



	//CanLockOn = false;


	//
	// 
	//for (int i = 0; i < ActorsLocked.Num(); i++)
	//{



	//	//error!!
	//	
	//}
	//
}

bool APlayerCharacter::CheckCanRespawn()
{
	if (GameMode->GetLives() > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void APlayerCharacter::HandleRespawn()
{
	if (CheckCanRespawn())
	{
		// Add respawn delay
		SetActorLocation(StartingLocation);
		SetActorHiddenInGame(false);
		SetActorTickEnabled(true);
		StartGhostPlayer();
		GetWorld()->GetTimerManager().SetTimer(ShotDelayTimerHandle, this, &APlayerCharacter::StopGhostPlayer, GhostingDuration, false);
	}
}

void APlayerCharacter::StartGhostPlayer()
{
	UE_LOG(LogTemp, Warning, TEXT("Start Ghost Player"));
}

void APlayerCharacter::StopGhostPlayer()
{
	UE_LOG(LogTemp, Warning, TEXT("Stop Ghost Player"));
}

void APlayerCharacter::RotateTargetingMesh()
{
	if (TargetingMesh)
	{
		float DeltaTime = GetWorld()->GetDeltaSeconds();

		// Get the current rotation
		FRotator CurrentRotation = TargetingMesh->GetComponentRotation();

		// Update the rotation (spin on the Y-axis)
		CurrentRotation.Yaw += TargetingMeshRotationSpeed * DeltaTime;

		// Set the new rotation
		TargetingMesh->SetWorldRotation(CurrentRotation);
	}
}

void APlayerCharacter::CheckPowerLevel()
{

}

void APlayerCharacter::ShotPower_1()
{
	//UE_LOG(LogTemp, Warning, TEXT("CanFire True, ShotPower_1 attempt"));
	FVector Location = ProjectileSpawnPoint->GetComponentLocation();
	FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();
	CreateMainProjectile(Location, Rotation);
}

void APlayerCharacter::ShotPower_2()
{
	FVector Location1 = ProjectileSpawnPoint->GetComponentLocation();
	Location1 += FVector(0.f, 20.f, 0.f);
	FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();
	CreateMainProjectile(Location1, Rotation);

	FVector Location2 = ProjectileSpawnPoint->GetComponentLocation();
	Location2 += FVector(0.f, -20.f, 0.f);
	CreateMainProjectile(Location2, Rotation);
}

void APlayerCharacter::ShotPower_3()
{
	FVector Location = ProjectileSpawnPoint->GetComponentLocation();
	FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();
	CreateMainProjectile(Location, Rotation);

	FVector Location1 = ProjectileSpawnPoint->GetComponentLocation();
	Location1 += FVector(-20.f, 20.f, 0.f);
	CreateMainProjectile(Location1, Rotation);

	FVector Location2 = ProjectileSpawnPoint->GetComponentLocation();
	Location2 += FVector(-20.f, -20.f, 0.f);
	CreateMainProjectile(Location2, Rotation);
}

void APlayerCharacter::ShotPower_4()
{
	FVector Location = ProjectileSpawnPoint->GetComponentLocation();
	FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();
	CreateMainProjectile(Location, Rotation);

	FVector Location1 = ProjectileSpawnPoint->GetComponentLocation();
	Location1 += FVector(-20.f, 20.f, 0.f);
	CreateMainProjectile(Location1, Rotation);

	FVector Location2 = ProjectileSpawnPoint->GetComponentLocation();
	Location2 += FVector(-20.f, -20.f, 0.f);
	CreateMainProjectile(Location2, Rotation);

	FRotator Rotation1 = ProjectileSpawnPoint->GetComponentRotation();
	Rotation1 += FRotator(0.f, -3.f, 0.f);
	CreateMainProjectile(Location, Rotation1);

	FRotator Rotation2 = ProjectileSpawnPoint->GetComponentRotation();
	Rotation2 += FRotator(0.f, 3.f, 0.f);
	CreateMainProjectile(Location, Rotation2);
}

void APlayerCharacter::ShotPower_5()
{
	FVector Location = ProjectileSpawnPoint->GetComponentLocation();
	FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();
	CreateMainProjectile(Location, Rotation);

	FVector Location1 = ProjectileSpawnPoint->GetComponentLocation();
	Location1 += FVector(-20.f, 20.f, 0.f);
	CreateMainProjectile(Location1, Rotation);

	FVector Location2 = ProjectileSpawnPoint->GetComponentLocation();
	Location2 += FVector(-20.f, -20.f, 0.f);
	CreateMainProjectile(Location2, Rotation);

	FRotator Rotation1 = ProjectileSpawnPoint->GetComponentRotation();
	Rotation1 += FRotator(0.f, -3.f, 0.f);
	CreateMainProjectile(Location, Rotation1);

	FRotator Rotation2 = ProjectileSpawnPoint->GetComponentRotation();
	Rotation2 += FRotator(0.f, 3.f, 0.f);
	CreateMainProjectile(Location, Rotation2);

	FRotator Rotation3 = ProjectileSpawnPoint->GetComponentRotation();
	Rotation3 += FRotator(0.f, -5.f, 0.f);
	CreateMainProjectile(Location, Rotation3);

	FRotator Rotation4 = ProjectileSpawnPoint->GetComponentRotation();
	Rotation4 += FRotator(0.f, 5.f, 0.f);
	CreateMainProjectile(Location, Rotation4);
}

void APlayerCharacter::CreateMainProjectile(FVector Location, FRotator Rotation)
{
	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
		MainProjectileClass,
		Location,
		Rotation
	);
	Projectile->SetOwner(this);
}

AProjectile* APlayerCharacter::CreateProjectile(UClass* ProjectileClass, FVector Location, FRotator Rotation)
{
	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
		ProjectileClass,
		Location,
		Rotation
	);
	Projectile->SetOwner(this);

	return Projectile;
}

AProjectile* APlayerCharacter::CreateLockOnProjectile(FVector Location, FRotator Rotation)
{
	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
		LockOnProjectileClass,
		Location,
		Rotation
	);
	Projectile->SetOwner(this);

	return Projectile;
}
