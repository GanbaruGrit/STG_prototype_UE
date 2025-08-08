// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "BaseEnemy.generated.h"


UENUM(BlueprintType)
enum class EFireType : uint8 {
	EFT_SINGLE UMETA(DisplayName = "Single"),
	EFT_DUAL UMETA(DisplayName = "Dual"),
	EFT_THREE_SPREAD UMETA(DisplayName = "Three Spread"),
	EFT_FIVE_SPREAD UMETA(DisplayName = "Five Spread")
};



UCLASS()
class STG_PROTOTYPE_API ABaseEnemy : public ABasePawn
{
	GENERATED_BODY()
	
public:
	void HandleDestruction();
    virtual void Tick(float DeltaTime) override;

protected:
    virtual void BeginPlay() override;

    virtual void Fire() override;

private:
    UPROPERTY()
    class APlayerCharacter* Player;

	bool DoShrink = false;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ShrinkRate = 10.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float RotationRate = 10.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireRange = 300.f;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireRate = 2.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float VolleyDelay = 3.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	bool EnableTurretRotation = true;

	UPROPERTY(EditAnywhere, Category = "Combat")
	EFireType CurrentFireType = EFireType::EFT_SINGLE;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	int MaxShotsInVolley = 3;

    UPROPERTY(VisibleInstanceOnly, Category = "Combat")
    int ShotsInVolleyCount = 0;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float DelayDestroySelf = 4.f;



	TArray<class UStaticMeshComponent*> StaticMeshCollection;
	FVector CurrentMeshScale;

	FTimerHandle FireRateTimerHandle;
	FTimerHandle VolleyDelayTimerHandle;
	FTimerHandle DestroySelfDelayTimerHandle;

	void CreateProjectile(FVector Location, FRotator Rotation);

	void FireSingle();

	void FireDual();

	void Fire3Way();

	void Fire5Way();

	UFUNCTION()
	void CheckFireCondition();

	UFUNCTION()
	void ResetVolleyCount();

	UFUNCTION()
	void CheckVolleyTimer();
	
    bool VolleyReady() const;

    bool InFireRange() const;

	void DestroySelf();

	void ShrinkMeshes();

	UFUNCTION(BlueprintCallable)
	void RotateXAxis();

};
