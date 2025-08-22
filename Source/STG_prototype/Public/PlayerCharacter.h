// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class UMaterialInterface;
class USceneComponent;
class UStaticMeshComponent;
class UCapsuleComponent;
class UNiagaraComponent;
class UStaticMesh;
class UCameraShakeBase;
class AProjectile;
class ASTGGameMode;
class APlayerController;
class LockOnHandler;
class USoundBase;
class UParticleSystem;
class UPlayerShootingComponent;

UCLASS()
class STG_PROTOTYPE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	
	void HandleDestruction();

protected:
	virtual void BeginPlay() override;

	// Enhanced input variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* PlayerMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* Fire1Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* Fire1ReleaseAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* Fire2Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LockOnAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LockOnReleaseAction;

	// Callbacks for input
	void Move(const FInputActionValue& Value);
	void PrimaryShot(const FInputActionValue& Value);
	void SecondaryShot(const FInputActionValue& Value);
	void ScanForLockOn(const FInputActionValue& Value);
	void Fire1ReleaseCharge(const FInputActionValue& Value);
	void LockOnRelease(const FInputActionValue& Value);

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PCTestFunc();

	UPROPERTY()
	ASTGGameMode* GameMode;

	UPROPERTY()
	APlayerController* STGPlayerController;
	
	APlayerController* GetPlayerController() const { return STGPlayerController; }

	// Shooting component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UPlayerShootingComponent* ShootingComponent;

	// Power-up functions
	void SetPowerLevel();
	void IncreasePowerLevel();

private:
	bool CanFire = true;
	bool CanCharge = true;
	bool CanLockOn = true;
	bool ScanningForLockOn = false;
	bool PrimaryShotFiring = false;

	LockOnHandler* LockOnHandlerInstance = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Respawn")
	float GhostingDuration = 3.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Respawn")
	float RespawnTime = 3.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float ShotDelay = 0.2f;

	UPROPERTY(EditDefaultsOnly, Category = "Charge")
	float TotalChargeTime = 3.0f;

	// Timer Handles
	FTimerHandle ShotDelayTimerHandle;
	FTimerHandle LockOnDelayTimerHandle;
	FTimerHandle ChargeOneTimerHandle;
	FTimerHandle ChargeTwoTimerHandle;
	FTimerHandle ChargeThreeTimerHandle;

	UFUNCTION()
	void ResetShotDelayTimer();
	
	UFUNCTION()
	void ResetLockOnDelayTimer();
	
	UFUNCTION()
	void EndChargeOneTimer();
	
	void EndChargeTwoTimer();
	void EndChargeThreeTimer();

	UFUNCTION(BlueprintCallable, Category = "Sockets")
	void AddSocket();

	UFUNCTION()
	void LockOn();
	
	void ParseLockOn();

	bool CheckCanRespawn();
	void HandleRespawn();
	void StartGhostPlayer();
	void StopGhostPlayer();

	FVector StartingLocation = FVector(70.0f, 180.0f, 0.0f);

	// Sockets and Ribbon Settings
	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
	USceneComponent* MainSocketComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
	USceneComponent* RibbonSocketComponentMiddle;

	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
	USceneComponent* RibbonSocketComponentMidLeft;

	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
	USceneComponent* RibbonSocketComponentMidRight;

	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
	USceneComponent* RibbonSocketComponentLeft;

	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
	USceneComponent* RibbonSocketComponentRight;

	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
	UStaticMesh* RibbonMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sockets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> RibbonBluePrint;

	UPROPERTY(EditAnywhere, Category = "Ribbon")
	UNiagaraComponent* NiagaraParticles;

	// Lock On Settings
	UPROPERTY(EditDefaultsOnly, Category = "LockOn")
	UStaticMeshComponent* TargetingMesh;

	UPROPERTY(EditAnywhere, Category = "LockOn")
	float TargetingMeshRotationSpeed = 50.0f;

	UPROPERTY(EditDefaultsOnly, Category = "LockOn")
	UMaterialInterface* LockOnMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "LockOn")
	FVector LockOnBoxHalfSize;

	UPROPERTY(EditDefaultsOnly, Category = "LockOn")
	FVector LockOnBoxEndRange;

	UPROPERTY(EditDefaultsOnly, Category = "LockOn")
	FVector LockOnBoxStartOffset;

	UPROPERTY(EditDefaultsOnly, Category = "LockOn")
	USceneComponent* LockOnSpawnPoint;

	UPROPERTY(EditDefaultsOnly, Category = "LockOn")
	USceneComponent* LockOnSpawnRotation;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float LockOnDelay = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "LockOn")
	int32 MaxLockOns = 8;

	UPROPERTY(EditAnywhere, Category = "LockOn")
	USoundBase* LockOnSound;

	TArray<AActor*> ActorsLocked;

	int32 i = 0;

	void RotateTargetingMesh();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CapsuleComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TurretMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditAnywhere, Category = "Combat")
	UParticleSystem* DeathParticles;

	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* DeathSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<UCameraShakeBase> DeathCameraShakeClass;
};
