// Copyright Epic Games, Inc. All Rights Reserved.

#include "PlayerShootingComponent.h"
#include "PlayerCharacter.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SceneComponent.h"

UPlayerShootingComponent::UPlayerShootingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlayerShootingComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerPlayer = Cast<APlayerCharacter>(GetOwner());
	if (OwnerPlayer)
	{
		// Find the projectile spawn point component
		ProjectileSpawnPoint = OwnerPlayer->FindComponentByClass<USceneComponent>();
	}
}

void UPlayerShootingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPlayerShootingComponent::StartPrimaryShot()
{
	if (bCanFire)
	{
		FirePrimaryShot();
	}
}

void UPlayerShootingComponent::StopPrimaryShot()
{
	// Stop charging if we were charging
	if (bIsCharging)
	{
		StopCharging();
	}
}

void UPlayerShootingComponent::FirePrimaryShot()
{
	if (!bCanFire) return;

	// Handle charge shots first
	if (CurrentChargeLevel != EChargePowerLevel::ECPL_1)
	{
		FireChargeShot();
		return;
	}

	// Fire based on current power level
	switch (CurrentFirePowerLevel)
	{
	case EFirePowerLevel::EFPL_1:
		ShotPower_1();
		break;
	case EFirePowerLevel::EFPL_2:
		ShotPower_2();
		break;
	case EFirePowerLevel::EFPL_3:
		ShotPower_3();
		break;
	case EFirePowerLevel::EFPL_4:
		ShotPower_4();
		break;
	case EFirePowerLevel::EFPL_5:
		ShotPower_5();
		break;
	}

	// Play sound
	if (PrimaryFireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PrimaryFireSound, GetOwner()->GetActorLocation());
	}

	// Set cooldown
	GetWorld()->GetTimerManager().SetTimer(ShotDelayTimerHandle, this, &UPlayerShootingComponent::ResetShotDelayTimer, ShotDelay, false);
	bCanFire = false;
}

void UPlayerShootingComponent::FireSecondaryShot()
{
	if (!bCanFire) return;

	FVector Location = GetProjectileSpawnLocation();
	FRotator Rotation = GetProjectileSpawnRotation();

	// Create three projectiles in a spread pattern
	CreateProjectile(SubProjectileClass, Location, Rotation);

	FRotator Rotation2 = Rotation + FRotator(0.0f, 15.0f, 0.0f);
	FVector Location2 = Location + FVector(0.0f, 10.0f, 0.0f);
	CreateProjectile(SubProjectileClass, Location2, Rotation2);

	FRotator Rotation3 = Rotation + FRotator(0.0f, -15.0f, 0.0f);
	FVector Location3 = Location + FVector(0.0f, -10.0f, 0.0f);
	CreateProjectile(SubProjectileClass, Location3, Rotation3);

	// Play sound
	if (SecondaryFireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, SecondaryFireSound, GetOwner()->GetActorLocation());
	}

	// Set cooldown
	GetWorld()->GetTimerManager().SetTimer(ShotDelayTimerHandle, this, &UPlayerShootingComponent::ResetShotDelayTimer, ShotDelay, false);
	bCanFire = false;
}

void UPlayerShootingComponent::SetFirePowerLevel(EFirePowerLevel NewLevel)
{
	CurrentFirePowerLevel = NewLevel;
}

void UPlayerShootingComponent::IncreaseFirePowerLevel()
{
	switch (CurrentFirePowerLevel)
	{
	case EFirePowerLevel::EFPL_1:
		CurrentFirePowerLevel = EFirePowerLevel::EFPL_2;
		break;
	case EFirePowerLevel::EFPL_2:
		CurrentFirePowerLevel = EFirePowerLevel::EFPL_3;
		break;
	case EFirePowerLevel::EFPL_3:
		CurrentFirePowerLevel = EFirePowerLevel::EFPL_4;
		break;
	case EFirePowerLevel::EFPL_4:
		CurrentFirePowerLevel = EFirePowerLevel::EFPL_5;
		break;
	case EFirePowerLevel::EFPL_5:
		// Already at max level
		break;
	}
}

void UPlayerShootingComponent::ResetFirePowerLevel()
{
	CurrentFirePowerLevel = EFirePowerLevel::EFPL_1;
}

void UPlayerShootingComponent::StartCharging()
{
	bIsCharging = true;
	CurrentChargeLevel = EChargePowerLevel::ECPL_1;

	// Set up charge timers
	GetWorld()->GetTimerManager().SetTimer(ChargeOneTimerHandle, this, &UPlayerShootingComponent::EndChargeOneTimer, 0.8f, false);
	GetWorld()->GetTimerManager().SetTimer(ChargeTwoTimerHandle, this, &UPlayerShootingComponent::EndChargeTwoTimer, 1.4f, false);
	GetWorld()->GetTimerManager().SetTimer(ChargeThreeTimerHandle, this, &UPlayerShootingComponent::EndChargeThreeTimer, 2.2f, false);
}

void UPlayerShootingComponent::StopCharging()
{
	bIsCharging = false;
	CurrentChargeLevel = EChargePowerLevel::ECPL_1;

	// Clear all charge timers
	GetWorld()->GetTimerManager().ClearTimer(ChargeOneTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(ChargeTwoTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(ChargeThreeTimerHandle);
}

void UPlayerShootingComponent::FireChargeShot()
{
	FVector Location = GetProjectileSpawnLocation();
	FRotator Rotation = GetProjectileSpawnRotation();
	AProjectile* ChargeShot = nullptr;

	switch (CurrentChargeLevel)
	{
	case EChargePowerLevel::ECPL_2:
		ChargeShot = CreateProjectile(ChargeOneProjectileClass, Location, Rotation);
		break;
	case EChargePowerLevel::ECPL_3:
		ChargeShot = CreateProjectile(ChargeTwoProjectileClass, Location, Rotation);
		break;
	case EChargePowerLevel::ECPL_4:
		ChargeShot = CreateProjectile(ChargeThreeProjectileClass, Location, Rotation);
		break;
	}

	if (ChargeShot)
	{
		ChargeShot->Tags.Add(FName("Charge Shot"));
	}

	// Reset charge level
	CurrentChargeLevel = EChargePowerLevel::ECPL_1;

	// Clear all charge timers
	GetWorld()->GetTimerManager().ClearTimer(ChargeOneTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(ChargeTwoTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(ChargeThreeTimerHandle);
}

void UPlayerShootingComponent::StartLockOnScanning()
{
	bScanningForLockOn = true;
}

void UPlayerShootingComponent::StopLockOnScanning()
{
	bScanningForLockOn = false;
}

void UPlayerShootingComponent::FireLockOnShots(const TArray<AActor*>& LockedTargets)
{
	if (LockedTargets.Num() == 0) return;

	FVector SpawnLocation = GetProjectileSpawnLocation();

	for (AActor* Target : LockedTargets)
	{
		if (Target)
		{
			FVector Direction = (Target->GetActorLocation() - SpawnLocation).GetSafeNormal();
			FRotator Rotation = Direction.Rotation();

			AProjectile* LockOnProjectile = CreateLockOnProjectile(SpawnLocation, Rotation);
			if (LockOnProjectile)
			{
				// Set the target for homing behavior
				LockOnProjectile->Tags.Add(FName("Lock On"));
			}
		}
	}

	// Play lock-on sound
	if (LockOnSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, LockOnSound, GetOwner()->GetActorLocation());
	}
}

void UPlayerShootingComponent::ShotPower_1()
{
	FVector Location = GetProjectileSpawnLocation();
	FRotator Rotation = GetProjectileSpawnRotation();
	CreateMainProjectile(Location, Rotation);
}

void UPlayerShootingComponent::ShotPower_2()
{
	FVector Location1 = GetProjectileSpawnLocation() + FVector(0.0f, 20.0f, 0.0f);
	FVector Location2 = GetProjectileSpawnLocation() + FVector(0.0f, -20.0f, 0.0f);
	FRotator Rotation = GetProjectileSpawnRotation();

	CreateMainProjectile(Location1, Rotation);
	CreateMainProjectile(Location2, Rotation);
}

void UPlayerShootingComponent::ShotPower_3()
{
	FVector BaseLocation = GetProjectileSpawnLocation();
	FRotator Rotation = GetProjectileSpawnRotation();

	CreateMainProjectile(BaseLocation, Rotation);
	CreateMainProjectile(BaseLocation + FVector(-20.0f, 20.0f, 0.0f), Rotation);
	CreateMainProjectile(BaseLocation + FVector(-20.0f, -20.0f, 0.0f), Rotation);
}

void UPlayerShootingComponent::ShotPower_4()
{
	FVector BaseLocation = GetProjectileSpawnLocation();
	FRotator BaseRotation = GetProjectileSpawnRotation();

	CreateMainProjectile(BaseLocation, BaseRotation);
	CreateMainProjectile(BaseLocation + FVector(-20.0f, 20.0f, 0.0f), BaseRotation);
	CreateMainProjectile(BaseLocation + FVector(-20.0f, -20.0f, 0.0f), BaseRotation);
	CreateMainProjectile(BaseLocation, BaseRotation + FRotator(0.0f, -3.0f, 0.0f));
	CreateMainProjectile(BaseLocation, BaseRotation + FRotator(0.0f, 3.0f, 0.0f));
}

void UPlayerShootingComponent::ShotPower_5()
{
	FVector BaseLocation = GetProjectileSpawnLocation();
	FRotator BaseRotation = GetProjectileSpawnRotation();

	CreateMainProjectile(BaseLocation, BaseRotation);
	CreateMainProjectile(BaseLocation + FVector(-20.0f, 20.0f, 0.0f), BaseRotation);
	CreateMainProjectile(BaseLocation + FVector(-20.0f, -20.0f, 0.0f), BaseRotation);
	CreateMainProjectile(BaseLocation, BaseRotation + FRotator(0.0f, -3.0f, 0.0f));
	CreateMainProjectile(BaseLocation, BaseRotation + FRotator(0.0f, 3.0f, 0.0f));
	CreateMainProjectile(BaseLocation, BaseRotation + FRotator(0.0f, -5.0f, 0.0f));
	CreateMainProjectile(BaseLocation, BaseRotation + FRotator(0.0f, 5.0f, 0.0f));
}

void UPlayerShootingComponent::CreateMainProjectile(FVector Location, FRotator Rotation)
{
	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
		MainProjectileClass,
		Location,
		Rotation
	);
	if (Projectile)
	{
		Projectile->SetOwner(GetOwner());
	}
}

AProjectile* UPlayerShootingComponent::CreateProjectile(UClass* ProjectileClass, FVector Location, FRotator Rotation)
{
	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
		ProjectileClass,
		Location,
		Rotation
	);
	if (Projectile)
	{
		Projectile->SetOwner(GetOwner());
	}
	return Projectile;
}

AProjectile* UPlayerShootingComponent::CreateLockOnProjectile(FVector Location, FRotator Rotation)
{
	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
		LockOnProjectileClass,
		Location,
		Rotation
	);
	if (Projectile)
	{
		Projectile->SetOwner(GetOwner());
	}
	return Projectile;
}

void UPlayerShootingComponent::ResetShotDelayTimer()
{
	bCanFire = true;
}

void UPlayerShootingComponent::EndChargeOneTimer()
{
	CurrentChargeLevel = EChargePowerLevel::ECPL_2;
}

void UPlayerShootingComponent::EndChargeTwoTimer()
{
	CurrentChargeLevel = EChargePowerLevel::ECPL_3;
}

void UPlayerShootingComponent::EndChargeThreeTimer()
{
	CurrentChargeLevel = EChargePowerLevel::ECPL_4;
}

FVector UPlayerShootingComponent::GetProjectileSpawnLocation() const
{
	if (ProjectileSpawnPoint)
	{
		return ProjectileSpawnPoint->GetComponentLocation();
	}
	return GetOwner()->GetActorLocation();
}

FRotator UPlayerShootingComponent::GetProjectileSpawnRotation() const
{
	if (ProjectileSpawnPoint)
	{
		return ProjectileSpawnPoint->GetComponentRotation();
	}
	return GetOwner()->GetActorRotation();
}
