// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "STGGameMode.generated.h"

/*
USTRUCT(BlueprintType)
struct FBaseGameStruct
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Sequence;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SequenceDuration;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName Scene;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int EnemyCount;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AActor> ActorArray;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ActorIndex;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector Locations;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Movements;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Health; // handled by comp?


	FBaseGameStruct()
	{
		Sequence = 1111;
	}
};
*/

UCLASS()
class STG_PROTOTYPE_API ASTGGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	void ActorDied(AActor* DeadActor);

	int GetLives();
	void ReduceLife();
	void AddLife();
	int GetScore();
	void AddToScore(int PointsToAdd);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Sequence;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SequenceDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName Scene;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int StructEnemyCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AActor*> ActorArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ActorIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector Locations;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Movements;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Health; // handled by comp?

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void StartGame();

	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool bWonGame);

private:
	class ABasePlayer* Player;
	class ASTGPlayerController* STGPlayerController;
	
	UPROPERTY(EditDefaultsOnly)
	float StartDelay = 3.f;

	// Core Stats
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	int Lives = 3;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	int Score = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float ScoreMultiplier = 1;

	void HandleGameStart();

	int32 TargetEnemies = 0;
	int32 GetTargetEnemiesCount();

	// Spawning
	int SpawnLoopCounter;
	float SpawnDelay;
	float SpawnVelocityMultiplier;
	// Create struct -> Struct data
	struct CurrentRowData;
	// Create array of enemy classes EnemyClasses
	// Create array of enemy objects EnemiesJustSpawned
	// string Scene
	// 

	//FBaseGameStruct BaseGameStruct;
};
