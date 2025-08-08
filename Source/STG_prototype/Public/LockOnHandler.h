// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/Actor.h"
#include "CoreMinimal.h"

/**
 * 
 */
class STG_PROTOTYPE_API LockOnHandler : public AActor
{
public:
	LockOnHandler();
	~LockOnHandler();

	bool ScanningForLockOn = false;
	bool CanLockOn = true;

	void ResetLockOnDelayTimer();
	void LockOn();
	void ParseLockOn();

protected:
	//virtual void BeginPlay() override;

private:

    UPROPERTY()
    APlayerCharacter* PlayerCharacterInstance;

	FTimerHandle LockOnDelayTimerHandle;

	// Lock On Settings
    UPROPERTY(EditDefaultsOnly, Category = "LockOn")
        UStaticMesh* TargetingMesh = nullptr;

    UPROPERTY(EditDefaultsOnly, Category = "LockOn")
        UMaterialInterface* LockOnMaterial = nullptr;

    UPROPERTY(EditDefaultsOnly, Category = "LockOn")
        FVector LockOnBoxHalfSize = FVector::ZeroVector;

    UPROPERTY(EditDefaultsOnly, Category = "LockOn")
        FVector LockOnBoxEndRange = FVector::ZeroVector;

    UPROPERTY(EditDefaultsOnly, Category = "LockOn")
        FVector LockOnBoxStartOffset = FVector::ZeroVector;

    UPROPERTY(EditDefaultsOnly, Category = "LockOn")
        USceneComponent* LockOnSpawnPoint = nullptr;

    UPROPERTY(EditDefaultsOnly, Category = "LockOn")
        USceneComponent* LockOnSpawnRotation = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
		float LockOnDelay = 1;

	UPROPERTY(EditDefaultsOnly, Category = "LockOn")
		int MaxLockOns = 8;

    UPROPERTY(EditAnywhere, Category = "LockOn")
        USoundBase* LockOnSound = nullptr;

	TArray<AActor*> ActorsLocked;

	int i = 0;

	

};
