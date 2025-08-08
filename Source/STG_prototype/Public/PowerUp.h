// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "BasePlayer.h"
#include "PowerUp.generated.h"

UCLASS()
class STG_PROTOTYPE_API APowerUp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APowerUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
    virtual void Tick(float DeltaTime) override;
	
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PowerUp", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PowerUp", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CapsuleComp;


private:
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
