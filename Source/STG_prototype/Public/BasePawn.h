// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"

class UCapsuleComponent;
class UStaticMeshComponent;
class USceneComponent;
class URotateToFollowPlayer;
class UNiagaraComponent;
class UNiagaraSystem;
class USoundBase;
class UCameraShakeBase;
class UParticleSystem;
class AActor;
class AProjectile;

UCLASS()
class STG_PROTOTYPE_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	ABasePawn();
	void HandleDestruction();

protected:
	virtual void BeginPlay() override;
	void RotateTurret(FVector LookAtTarget);
	virtual void Fire();

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USplineMovementComponent* SplineMovementComponent;*/

	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TSubclassOf<AActor> SplineMovementActor;*/

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USplineComponent* SplineComponent;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> SplineBlueprint;

public:	
    virtual void Tick(float DeltaTime) override;

	/*virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;*/

protected:
    UPROPERTY()
    class ASTGGameMode* GameMode;
	
	bool CanFire = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* DefaultRootComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* CylinderMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ConeMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TurretMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TestMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* TurretSceneComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	URotateToFollowPlayer* RotateToFollowPlayerComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* RotateToFollowPlayerMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float TurretRotationRate = 10.f;

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
	class UNiagaraComponent* DeathParticlesNiagaraComponent;
	UPROPERTY(EditAnywhere, Category = "Combat")
	class UNiagaraSystem* DeathParticlesNiagaraSystem;

	UPROPERTY(EditAnywhere, Category = "Combat")
	class USoundBase* DeathSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<class UCameraShakeBase> DeathCameraShakeClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	USoundBase* PrimaryFireSound;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	USoundBase* SecondaryFireSound;

	UPROPERTY(EditAnywhere, Category = "Break")
	int NumBreakableMeshes = 8;
	
	UPROPERTY(EditAnywhere, Category = "Break")
	FVector AddForce = FVector(500.f, 0.f, 0.f);

	UPROPERTY(EditAnywhere, Category = "Break")
	FVector AddImpulse = FVector(0.f, 0.f, 0.f);

	UPROPERTY(EditAnywhere, Category = "Break")
	TArray<class UStaticMeshComponent*> BreakableMeshComponentCollection;

	void FallApart();
	void CreateBreakableMeshes();

	// Make function for collection breakable mesh
};
