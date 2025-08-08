// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReactiveSphere.generated.h"

class USphereComponent;
class UNiagaraComponent;
class UParticleSystem;
class USoundBase;
class UCameraShakeBase;

UCLASS()
class STG_PROTOTYPE_API AReactiveSphere : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AReactiveSphere();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USphereComponent* SphereComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;

	FTimerHandle DelayDestroyTimerHandle;
	FTimerHandle DelayScaleTimerHandle;

    bool bCanScale = false;

	UPROPERTY(EditAnywhere, Category = "Scale")
	float ScaleDelay = 1.f;

	UPROPERTY(EditAnywhere, Category = "Scale")
    float StartScale = 1.f;

	UPROPERTY(EditAnywhere, Category = "Scale")
    float TargetScale = 2.f;

	UPROPERTY(EditAnywhere, Category = "Scale")
    float ScaleRate = 1.f;

    float CurrentTime = 0.f;

    float TotalTime = 1.f;

	UPROPERTY(EditAnywhere, Category = "Particles")
	class UNiagaraComponent* NiagaraParticles;

	UPROPERTY(EditAnywhere, Category = "Particles")
	class UParticleSystem* CascadeParticles;
	
	UPROPERTY(EditAnywhere, Category = "Particles")
	TSubclassOf<UObject> BP_Effect;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	float DestroyDelay = 3.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float TimeToLive = 1;

	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* LaunchSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<class UCameraShakeBase> HitCameraShakeClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float ImpulseForce;

	void ScaleMeshOverTime(bool bScale);

	UFUNCTION()
	void DestroySelf();

	UFUNCTION()
	void BeginScaling();

	/*UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);*/

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
