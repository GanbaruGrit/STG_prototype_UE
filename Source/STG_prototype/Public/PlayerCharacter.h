// Fill out your copyright notice in the Description page of Project Settings.

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

UENUM(BlueprintType)
enum class EFirePowerLevel : uint8 {
	EFPL_1 UMETA(DisplayName = "Fire Power: 1"),
	EFPL_2 UMETA(DisplayName = "Fire Power: 2"),
	EFPL_3 UMETA(DisplayName = "Fire Power: 3"),
	EFPL_4 UMETA(DisplayName = "Fire Power: 4"),
	EFPL_5 UMETA(DisplayName = "Fire Power: 5")
};

UENUM(BlueprintType)
enum class EChargePowerLevel : uint8 {
	ECPL_1 UMETA(DisplayName = "Charge Power: 0"),
	ECPL_2 UMETA(DisplayName = "Charge Power: 1"),
	ECPL_3 UMETA(DisplayName = "Charge Power: 2"),
	ECPL_4 UMETA(DisplayName = "Charge Power: 3"),
};


UCLASS()
class STG_PROTOTYPE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	void HandleDestruction();

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	//float PlayerSpeed = 1.f;

	

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


	/*void Look(const FInputActionValue& Value);
	void EKeyPressed();
	virtual void Attack() override;*/


public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PCTestFunc();

    UPROPERTY()
    class ASTGGameMode* GameMode;

    UPROPERTY()
    APlayerController* STGPlayerController;
	APlayerController* GetPlayerController() const { return STGPlayerController; }

	// Getters/Setters
	EFirePowerLevel GetPowerLevel();
	void SetPowerLevel();

	EChargePowerLevel GetChargeLevel();
	void SetChargeLevel(EChargePowerLevel NewChargeLevel);

    AProjectile* CreateLockOnProjectile(FVector Location, FRotator Rotation);

private:
    bool CanFire = true;
    bool CanCharge = true;
    bool CanLockOn = true;
    bool ScanningForLockOn = false;
    bool PrimaryShotFiring = false;

    class LockOnHandler* LockOnHandlerInstance = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Respawn")
	float GhostingDuration = 3;

	UPROPERTY(EditDefaultsOnly, Category = "Respawn")
    float RespawnTime = 3.f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
    float ShotDelay = 0.2f;

	UPROPERTY(EditDefaultsOnly, Category = "Charge")
	float TotalChargeTime = 3;

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
	

	FVector StartingLocation = FVector(70.f, 180.f, 0.f);

	/*UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;*/

	UPROPERTY(EditAnywhere, Category = "Combat")
	EFirePowerLevel CurrentFireType = EFirePowerLevel::EFPL_1;

	UPROPERTY(EditAnywhere, Category = "Combat")
	int CurrentPowerLevel = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Charge")
	EChargePowerLevel CurrentChargeType = EChargePowerLevel::ECPL_1;

	int CurrentChargeLevel = 0;

	// Sockets and Ribbon Settings
	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
	class USceneComponent* MainSocketComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
	class USceneComponent* RibbonSocketComponentMiddle;

	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
	class USceneComponent* RibbonSocketComponentMidLeft;

	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
	class USceneComponent* RibbonSocketComponentMidRight;

	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
	class USceneComponent* RibbonSocketComponentLeft;

	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
	class USceneComponent* RibbonSocketComponentRight;

	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
	class UStaticMesh* RibbonMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sockets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> RibbonBluePrint;

	UPROPERTY(EditAnywhere, Category = "Ribbon")
	class UNiagaraComponent* NiagaraParticles;

	//// Lock On Settings
	UPROPERTY(EditDefaultsOnly, Category = "LockOn")
	UStaticMeshComponent* TargetingMesh;

	UPROPERTY(EditAnywhere, Category = "LockOn")
	float TargetingMeshRotationSpeed = 50.f;

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
	float LockOnDelay = 1;

	UPROPERTY(EditDefaultsOnly, Category = "LockOn")
	int MaxLockOns = 8;

	UPROPERTY(EditAnywhere, Category = "LockOn")
	USoundBase* LockOnSound;



	TArray<AActor*> ActorsLocked;

    int i = 0;


	void RotateTargetingMesh();

	

	void CheckPowerLevel();

	void ShotPower_1();

	void ShotPower_2();

	void ShotPower_3();

	void ShotPower_4();

	void ShotPower_5();

	void CreateMainProjectile(FVector Location, FRotator Rotation);
	AProjectile* CreateProjectile(UClass* ProjectileClass, FVector Location, FRotator Rotation);
	

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* RootComponent;*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TurretMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AProjectile> MainProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AProjectile> SubProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AProjectile> ChargeOneProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AProjectile> ChargeTwoProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AProjectile> ChargeThreeProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AProjectile> LockOnProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Combat")
	class UParticleSystem* DeathParticles;

	UPROPERTY(EditAnywhere, Category = "Combat")
	class USoundBase* DeathSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<class UCameraShakeBase> DeathCameraShakeClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	USoundBase* PrimaryFireSound;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	USoundBase* SecondaryFireSound;
};
