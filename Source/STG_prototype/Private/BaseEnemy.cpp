// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "BasePlayer.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "PlayerCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Projectile.h"


void ABaseEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    /*if (InFireRange() && EnableTurretRotation)
    {
        RotateTurret(Player->GetActorLocation());
    }*/

    if (DoShrink)
    {
        ShrinkMeshes();
    }
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

    // Mark enemy as lockable once at start instead of every tick
    Tags.AddUnique(FName("Lockable"));

	GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ABaseEnemy::CheckFireCondition, FireRate, true);

	
	GetComponents<UStaticMeshComponent>(StaticMeshCollection);

	for (UStaticMeshComponent* StaticMesh : StaticMeshCollection)
	{
		UE_LOG(LogTemp, Warning, TEXT("StaticMeshCollection: %s"), *StaticMesh->GetName());

		/**UGameplayStatics::GetWorldDeltaSeconds(this)*/
	}
	
}

void ABaseEnemy::Fire()
{
	if (CurrentFireType == EFireType::EFT_SINGLE)
	{
		FireSingle();
	}

	if (CurrentFireType == EFireType::EFT_DUAL)
	{
		FireDual();
	}

	if (CurrentFireType == EFireType::EFT_THREE_SPREAD)
	{
		Fire3Way();
	}

	if (CurrentFireType == EFireType::EFT_FIVE_SPREAD)
	{
		Fire5Way();
	}
}

void ABaseEnemy::CreateProjectile(FVector Location, FRotator Rotation)
{
    if (MainProjectileClass == nullptr)
    {
        return;
    }
	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
		MainProjectileClass,
		Location,
		Rotation
	);

	Projectile->SetOwner(this);
}

void ABaseEnemy::FireSingle()
{
	FVector Location = ProjectileSpawnPoint->GetComponentLocation();
	FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();
	CreateProjectile(Location, Rotation);
	ShotsInVolleyCount++;
	CheckVolleyTimer();
}

void ABaseEnemy::FireDual()
{
	FVector Location1 = ProjectileSpawnPoint->GetComponentLocation();
	Location1 += FVector(0.f, 40.f, 0.f);
	FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();
	CreateProjectile(Location1, Rotation);

	FVector Location2 = ProjectileSpawnPoint->GetComponentLocation();
	Location2 += FVector(0.f, -40.f, 0.f);
	CreateProjectile(Location2, Rotation);

	ShotsInVolleyCount++;
	CheckVolleyTimer();
}

void ABaseEnemy::Fire3Way()
{
	FVector Location = ProjectileSpawnPoint->GetComponentLocation();
	FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();
	CreateProjectile(Location, Rotation);
	
	FRotator Rotation1 = ProjectileSpawnPoint->GetComponentRotation();
	Rotation1 += FRotator(0.f, 15.f, 0.f);
	CreateProjectile(Location, Rotation1);

	FRotator Rotation2 = ProjectileSpawnPoint->GetComponentRotation();
	Rotation2 += FRotator(0.f, -15.f, 0.f);
	CreateProjectile(Location, Rotation2);

	ShotsInVolleyCount++;
	CheckVolleyTimer();
}

void ABaseEnemy::Fire5Way()
{
	FVector Location = ProjectileSpawnPoint->GetComponentLocation();
	FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();
	CreateProjectile(Location, Rotation);

	FRotator Rotation1 = ProjectileSpawnPoint->GetComponentRotation();
	Rotation1 += FRotator(0.f, 15.f, 0.f);
	CreateProjectile(Location, Rotation1);

	FRotator Rotation2 = ProjectileSpawnPoint->GetComponentRotation();
	Rotation2 += FRotator(0.f, -15.f, 0.f);
	CreateProjectile(Location, Rotation2);

	FRotator Rotation3 = ProjectileSpawnPoint->GetComponentRotation();
	Rotation3 += FRotator(0.f, 25.f, 0.f);
	CreateProjectile(Location, Rotation3);

	FRotator Rotation4 = ProjectileSpawnPoint->GetComponentRotation();
	Rotation4 += FRotator(0.f, -25.f, 0.f);
	CreateProjectile(Location, Rotation4);

	ShotsInVolleyCount++;
	CheckVolleyTimer();
}

void ABaseEnemy::CheckFireCondition()
{
	if (InFireRange() && VolleyReady())
	{
		Fire();
	}
}

bool ABaseEnemy::VolleyReady() const
{
	if (ShotsInVolleyCount < MaxShotsInVolley)
	{
		//UE_LOG(LogTemp, Warning, TEXT("VolleyReady: True"));
		return true;
	}
	
	return false;
}

void ABaseEnemy::CheckVolleyTimer()
{
	if (ShotsInVolleyCount >= MaxShotsInVolley)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Volley Ready Timer Segment hit!"));
		GetWorldTimerManager().SetTimer(VolleyDelayTimerHandle, this, &ABaseEnemy::ResetVolleyCount, VolleyDelay, false);
	}
}

void ABaseEnemy::ResetVolleyCount()
{
	//UE_LOG(LogTemp, Warning, TEXT("Reset Volley Count Hit!"));
	ShotsInVolleyCount = 0;
}

bool ABaseEnemy::InFireRange() const
{
	if ((Player) && (Player->IsHidden() == false))
	{
		float Distance = FVector::Dist(GetActorLocation(), Player->GetActorLocation()); // Check distance to player
		if (Distance <= FireRange)
		{
            return true;
		}
	}

	return false;
}

void ABaseEnemy::DestroySelf()
{
	Destroy();
}

void ABaseEnemy::ShrinkMeshes()
{
	
	for (UStaticMeshComponent* StaticMesh : StaticMeshCollection)
	{
		float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
		CurrentMeshScale = StaticMesh->GetComponentScale();
		UE_LOG(LogTemp, Warning, TEXT("Current Scale: %s"), *CurrentMeshScale.ToString());
		FVector NewMeshScale = CurrentMeshScale - FVector(ShrinkRate, ShrinkRate, ShrinkRate) * DeltaTime;
		UE_LOG(LogTemp, Warning, TEXT("New Scale: %s"), *NewMeshScale.ToString());
		StaticMesh->SetWorldScale3D(NewMeshScale);

		if (NewMeshScale.X <= 0.f && NewMeshScale.Y <= 0.f && NewMeshScale.Z <= 0.f) {
			DoShrink = false;
		}

		
	}
}

void ABaseEnemy::RotateXAxis()
{
	float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	/*FRotator CurrentRootRotation = RootComponent->GetComponentRotation();
	FRotator NewRootRotation = CurrentRootRotation + FRotator(RotationRate, 0.f, 0.f) * DeltaTime;
	RootComponent->SetWorldRotation(NewRootRotation);*/

	FRotator NewRotation = GetActorRotation();
	NewRotation.Roll += RotationRate * DeltaTime; // Adjust RotationSpeed as needed
	SetActorRotation(NewRotation);
}



void ABaseEnemy::HandleDestruction()
{
	Super::HandleDestruction();

	//DoShrink = true;

	GetWorldTimerManager().SetTimer(DestroySelfDelayTimerHandle, this, &ABaseEnemy::DestroySelf, DelayDestroySelf, false);
}