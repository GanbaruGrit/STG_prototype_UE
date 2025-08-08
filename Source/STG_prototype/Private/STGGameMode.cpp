// Fill out your copyright notice in the Description page of Project Settings.


#include "STGGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "BasePlayer.h"
#include "BaseEnemy.h"
#include "STGPlayerController.h"

void ASTGGameMode::ActorDied(AActor* DeadActor)
{
	if (DeadActor == Player)
	{
		Player->HandleDestruction();

		if (STGPlayerController)
		{
			//STGPlayerController->SetPlayerEnabledState(false);
			
		}
		//GameOver(false);
	}
	else if (ABaseEnemy* DestroyedEnemy = Cast<ABaseEnemy>(DeadActor)) // Check if enemy via cast and store to var
	{
		DestroyedEnemy->HandleDestruction();
		TargetEnemies--;
		if (TargetEnemies == 0)
		{
			GameOver(true);
		}
	}
}

int ASTGGameMode::GetLives()
{
	return Lives;
}

void ASTGGameMode::ReduceLife()
{
	Lives--;
	UE_LOG(LogTemp, Warning, TEXT("Life Reduced, new Lifes:  %d"), Lives);
}

void ASTGGameMode::AddLife()
{
	Lives++;
}

int ASTGGameMode::GetScore()
{
	return Score;
}

void ASTGGameMode::AddToScore(int PointsToAdd)
{
	Score += PointsToAdd;
}

void ASTGGameMode::BeginPlay()
{
	Super::BeginPlay();

	HandleGameStart();

	//UE_LOG(LogTemp, Warning, TEXT("From Game Mode in BasePlayer: %d"), BaseGameStruct.Sequence);
	

}

void ASTGGameMode::HandleGameStart()
{
	TargetEnemies = GetTargetEnemiesCount();
	
	Player =  Cast<ABasePlayer>(UGameplayStatics::GetPlayerPawn(this, 0)); // Get a reference to the player by fetching the pawn and casting to the BasePlayer type
	STGPlayerController = Cast<ASTGPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	StartGame();

	if (STGPlayerController)
	{
		STGPlayerController->SetPlayerEnabledState(false);

		FTimerHandle PlayerEnableTimerHandle;

		FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject(
			STGPlayerController,
			&ASTGPlayerController::SetPlayerEnabledState,
			true
			);
		GetWorldTimerManager().SetTimer(
			PlayerEnableTimerHandle,
			PlayerEnableTimerDelegate,
			StartDelay,
			false
		);
	}
}

int32 ASTGGameMode::GetTargetEnemiesCount()
{
	TArray<AActor*> EnemyCount;

	UGameplayStatics::GetAllActorsOfClass(this, ABaseEnemy::StaticClass(), EnemyCount);
	
	return EnemyCount.Num();
}
