// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerShootingComponent.generated.h"

class AProjectile;
class USoundBase;
class USceneComponent;
class APlayerCharacter;

UENUM(BlueprintType)
enum class EFirePowerLevel : uint8
{
	EFPL_1 UMETA(DisplayName = "Fire Power: 1"),
	EFPL_2 UMETA(DisplayName = "Fire Power: 2"),
	EFPL_3 UMETA(DisplayName = "Fire Power: 3"),
	EFPL_4 UMETA(DisplayName = "Fire Power: 4"),
	EFPL_5 UMETA(DisplayName = "Fire Power: 5")
};

UENUM(BlueprintType)
enum class EChargePowerLevel : uint8
{
	ECPL_1 UMETA(DisplayName = "Charge Power: 0"),
	ECPL_2 UMETA(DisplayName = "Charge Power: 1"),
	ECPL_3 UMETA(DisplayName = "Charge Power: 2"),
	ECPL_4 UMETA(DisplayName = "Charge Power: 3")
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class STG_PROTOTYPE_API UPlayerShootingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerShootingComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Primary shooting functions
	void StartPrimaryShot();
	void StopPrimaryShot();
	void FirePrimaryShot();
	void FireSecondaryShot();

	// Power level management
	EFirePowerLevel GetCurrentFirePowerLevel() const { return CurrentFirePowerLevel; }
	void SetFirePowerLevel(EFirePowerLevel NewLevel);
	void IncreaseFirePowerLevel();
	void ResetFirePowerLevel();

	// Charge shot management
	EChargePowerLevel GetCurrentChargeLevel() const { return CurrentChargeLevel; }
	void StartCharging();
	void StopCharging();
	void FireChargeShot();

	// Lock-on system
	void StartLockOnScanning();
	void StopLockOnScanning();
	void FireLockOnShots(const TArray<AActor*>& LockedTargets);

	// Getters for PlayerCharacter
	bool CanFire() const { return bCanFire; }
	bool IsCharging() const { return bIsCharging; }
	bool IsScanningForLockOn() const { return bScanningForLockOn; }

protected:
	// Component references
	UPROPERTY()
	APlayerCharacter* OwnerPlayer;

	UPROPERTY()
	USceneComponent* ProjectileSpawnPoint;

	// Shooting state
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	bool bCanFire = true;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float ShotDelay = 0.2f;

	UPROPERTY(EditDefaultsOnly, Category = "Charge")
	float TotalChargeTime = 3.0f;

	// Power levels
	UPROPERTY(EditAnywhere, Category = "Combat")
	EFirePowerLevel CurrentFirePowerLevel = EFirePowerLevel::EFPL_1;

	UPROPERTY(EditDefaultsOnly, Category = "Charge")
	EChargePowerLevel CurrentChargeLevel = EChargePowerLevel::ECPL_1;

	// Charge shot state
	bool bIsCharging = false;
	bool bIsScanningForLockOn = false;

	// Timer handles
	FTimerHandle ShotDelayTimerHandle;
	FTimerHandle ChargeOneTimerHandle;
	FTimerHandle ChargeTwoTimerHandle;
	FTimerHandle ChargeThreeTimerHandle;

	// Projectile classes
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AProjectile> MainProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AProjectile> SubProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AProjectile> ChargeOneProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AProjectile> ChargeTwoProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AProjectile> ChargeThreeProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AProjectile> LockOnProjectileClass;

	// Sound effects
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	USoundBase* PrimaryFireSound;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	USoundBase* SecondaryFireSound;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	USoundBase* LockOnSound;

private:
	// Internal shooting functions
	void ShotPower_1();
	void ShotPower_2();
	void ShotPower_3();
	void ShotPower_4();
	void ShotPower_5();

	void CreateMainProjectile(FVector Location, FRotator Rotation);
	AProjectile* CreateProjectile(UClass* ProjectileClass, FVector Location, FRotator Rotation);
	AProjectile* CreateLockOnProjectile(FVector Location, FRotator Rotation);

	// Timer callbacks
	UFUNCTION()
	void ResetShotDelayTimer();

	UFUNCTION()
	void EndChargeOneTimer();

	void EndChargeTwoTimer();
	void EndChargeThreeTimer();

	// Helper functions
	FVector GetProjectileSpawnLocation() const;
	FRotator GetProjectileSpawnRotation() const;
};
