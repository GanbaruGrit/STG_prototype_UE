// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "ReactiveExplosion.h"
#include "Projectile.generated.h"

UCLASS()
class STG_PROTOTYPE_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	class UProjectileMovementComponent* CoreProjectileMovementComponent;


private:
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditDefaultsOnly)
	class UCapsuleComponent* CapsuleComp;


    UPROPERTY(EditDefaultsOnly, Category = "Movement")
    float InitialSpeed = 1500.f;
    UPROPERTY(EditDefaultsOnly, Category = "Movement")
    float MaxSpeed = 1500.f;
    UPROPERTY(EditDefaultsOnly, Category = "Movement")
    float ImpulseForce = 0.f;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere)
	float Damage = 10.f;

    FVector CurrentPlayerLocation = FVector::ZeroVector;
    FVector CurrentPlayerRotation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Combat")
	UParticleSystem* HitParticles;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<class AReactiveSphere> ReactiveSphere;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<class AReactiveSphere> ReactiveSphere2nd;

	UPROPERTY(EditAnywhere, Category = "Combat")
	class UNiagaraComponent* NiagaraParticles;

	UPROPERTY(EditAnywhere, Category = "Combat")
	class UParticleSystem* CascadeParticles;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FRotator AltRotation = FRotator(0.f, 90.f, 0.f);

	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* LaunchSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<class UCameraShakeBase> HitCameraShakeClass;
};
