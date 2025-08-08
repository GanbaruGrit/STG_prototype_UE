// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "LockOnHandler.h"

LockOnHandler::LockOnHandler()
{
}

LockOnHandler::~LockOnHandler()
{
}

void LockOnHandler::ResetLockOnDelayTimer()
{
	ParseLockOn();

	CanLockOn = true;
}

void LockOnHandler::LockOn()
{
	//UKismetSystemLibrary::BoxTraceMulti(GetWorld(), Start, End, BoxHalfSize, BoxRotation, TraceChannel, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, OutHits, true);

	FVector Start = GetActorLocation();
	/*FVector PlayerStart = GetActorLocation();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Hit Result: %s"), *PlayerStart.ToString()));*/
	FVector End = Start += LockOnBoxEndRange;
	FRotator BoxRotation = FRotator(0.f, 0.f, 0.f);
	ETraceTypeQuery TraceChannel = ETraceTypeQuery::TraceTypeQuery2;
	TArray<FHitResult> OutHits;
	TArray<AActor*> ActorsToIgnore;

	bool isHit = UKismetSystemLibrary::BoxTraceMulti(GetWorld(), Start, End, LockOnBoxHalfSize, BoxRotation, TraceChannel, false, ActorsToIgnore, EDrawDebugTrace::None, OutHits, true);


	if (isHit)
	{

		for (auto& Hit : OutHits)
		{
			AActor* HitActor = Hit.GetActor();


			if (HitActor->ActorHasTag("Lockable"))
			{
				Hit.GetComponent()->SetMaterial(0, LockOnMaterial);

				ActorsLocked.AddUnique(HitActor);
				AActor* TempActor = ActorsLocked.Last();

				if (GEngine)
				{
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Hit Result: %s"), TempActor));
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("All Hit Information: %s"), *Hit.ToString()));
				}

				//UE_LOG(LogTemp, Warning, TEXT("ActorsLocked: "), ActorsLocked[0];

				/*AProjectile* LockOnProjectile = CreateLockOnProjectile(LockOnSpawnPoint->GetComponentLocation(), LockOnSpawnPoint->GetComponentRotation());
				UProjectileMovementComponent* LockOnProjectileMovement = Cast<UProjectileMovementComponent>(LockOnProjectile->GetComponentByClass(UProjectileMovementComponent::StaticClass()));

				LockOnProjectileMovement->HomingTargetComponent = HitActor->GetRootComponent();*/

				//CanLockOn = false;
				//GetWorld()->GetTimerManager().SetTimer(LockOnDelayTimerHandle, this, &ABasePlayer::ResetLockOnDelayTimer, LockOnDelay, false);
			}


			/*GetWorld()->SpawnActor<UStaticMesh>(
				TargetingMesh,
				Location,
				Rotation
			);
			Projectile->SetOwner(this);*/

		}
	}

	/*CanLockOn = false;
		GetWorld()->GetTimerManager().SetTimer(LockOnDelayTimerHandle, this, &ABasePlayer::ResetLockOnDelayTimer, LockOnDelay, false);*/

}

void LockOnHandler::ParseLockOn()
{
	//UE_LOG(LogTemp, Warning, TEXT("LockOn Array Index: %d"), i);



	if (i < ActorsLocked.Num())
	{
		FString ActorName = ActorsLocked[i]->GetName();
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("LockOn Array Result: %s"), ActorName));
		//UE_LOG(LogTemp, Warning, TEXT("LockOn Array Result: %s"), *ActorName);
		if (ActorsLocked[i]->ActorHasTag("Lockable"))

			if (GEngine)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("LockOn Array Result: %s"), ActorsLocked[i]));
				// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("All Hit Information: %s"), *Hit.ToString()));
			}

		{
			AProjectile* LockOnProjectile = PlayerCharacterInstance->CreateLockOnProjectile(LockOnSpawnPoint->GetComponentLocation(), LockOnSpawnPoint->GetComponentRotation());
			UProjectileMovementComponent* LockOnProjectileMovement = Cast<UProjectileMovementComponent>(LockOnProjectile->GetComponentByClass(UProjectileMovementComponent::StaticClass()));

			LockOnProjectileMovement->HomingTargetComponent = ActorsLocked[i]->GetRootComponent();

			if (LockOnSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, LockOnSound, GetActorLocation());
			}

			i++;
			GetWorld()->GetTimerManager().SetTimer(LockOnDelayTimerHandle, this, &LockOnHandler::ResetLockOnDelayTimer, LockOnDelay, false);
		}

	}
	else
	{
		ActorsLocked.Empty();
		i = 0;
	}



	//CanLockOn = false;


	//
	// 
	//for (int i = 0; i < ActorsLocked.Num(); i++)
	//{



	//	//error!!
	//	
	//}
	//
}

//void LockOnHandler::BeginPlay()
//{
//	Super::BeginPlay();
//
//}
//
//void LockOnHandler::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//	if (ScanningForLockOn)
//	{
//		LockOn();
//	}
//}