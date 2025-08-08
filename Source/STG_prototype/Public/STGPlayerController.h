// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "STGPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class STG_PROTOTYPE_API ASTGPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void SetPlayerEnabledState(bool bPlayerEnabled);

};
