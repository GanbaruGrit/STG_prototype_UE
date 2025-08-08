// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayer.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"
#include "STGGameMode.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Projectile.h"
#include "Particles/ParticleSystem.h"
#include "DrawDebugHelpers.h"

ABasePlayer::ABasePlayer()
{
	
}

