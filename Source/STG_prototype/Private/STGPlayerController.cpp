
// Fill out your copyright notice in the Description page of Project Settings.


#include "STGPlayerController.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"

void ASTGPlayerController::SetPlayerEnabledState(bool bPlayerEnabled)
{
    if (bPlayerEnabled)
    {
        if (APawn* Pawn = GetPawn())
        {
            Pawn->EnableInput(this);
        }
    }
    else
    {
        if (APawn* Pawn = GetPawn())
        {
            Pawn->DisableInput(this);
        }
    }
}
