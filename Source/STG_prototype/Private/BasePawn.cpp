#include "BasePawn.h"
#include "STGGameMode.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Projectile.h"
#include "RotateToFollowPlayer.h"
#include "Particles/ParticleSystem.h"

ABasePawn::ABasePawn()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set this pawn to call Tick() every frame
	//AutoPossessPlayer = EAutoReceiveInput::Player0;

	/*DefaultRootComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Default Scene Root"));
	SetRootComponent(DefaultRootComponent);*/

	//TestMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Test Mesh"));

	/*CylinderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cylinder Mesh"));
	CylinderMesh->SetupAttachment(DefaultRootComponent);
	
	ConeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cone Mesh"));
	ConeMesh->SetupAttachment(CylinderMesh);*/

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	SetRootComponent(CapsuleComp);

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComp);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	TurretSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Turret Scene Component"));
	TurretSceneComponent->SetupAttachment(CapsuleComp);

	RotateToFollowPlayerComponent = CreateDefaultSubobject<URotateToFollowPlayer>(TEXT("Rotate To Follow Player Component"));
	RotateToFollowPlayerComponent->SetupAttachment(CapsuleComp);

	RotateToFollowPlayerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rotate To Follow Player Mesh"));
	RotateToFollowPlayerMesh->SetupAttachment(RotateToFollowPlayerComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(RotateToFollowPlayerComponent);

	/*DeathParticlesNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara Death Particles Component"));
	DeathParticlesNiagaraComponent->SetupAttachment(BaseMesh);
	DeathParticlesNiagaraComponent->DeactivateImmediate();*/

	//CreateBreakableMeshes();

	if (SplineBlueprint)
	{
		// You can use any unique name for the subobject, for example, "SplineComponent"
		USplineComponent* SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent From Base Pawn -> Spline BP"));
		SplineComponent->SetupAttachment(CapsuleComp);
		// Check if the subobject was successfully created
		if (SplineComponent)
		{
			// Set other properties or perform additional setup if needed
			// For example: SplineComponent->SomeFunction();
		}
		else
		{
			// Handle the case where the subobject creation failed
		}
	}

	FVector PSPLoc = ProjectileSpawnPoint->GetComponentLocation();

	//UE_LOG(LogTemp, Warning, TEXT("Initial Proj Spawn Point in Constructor %s"), *PSPLoc.ToString

}

void ABasePawn::BeginPlay()
{
	Super::BeginPlay();

	//if (SplineMovementComponent) 
	//{
	//	SplineMovementComponent->SetSplineComponent(SplineComponent);
	//	
	//	//SetActorLocation(SplineMovementComponent->GetSplineStartLocation());
	//}
	
	GameMode = Cast<ASTGGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		int TestNum = GameMode->GetLives();

	}

}

void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	/*if (SplineMovementComponent)
	{
		SplineMovementComponent->MoveAlongSpline();

		SetActorLocation(SplineMovementComponent->GetSplineStartLocation());
	}*/
}

void ABasePawn::FallApart()
{
	for (auto& MeshComponent : BreakableMeshComponentCollection)
	{
		MeshComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		MeshComponent->SetSimulatePhysics(true);
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		MeshComponent->AddForce(FVector (500.f, 0.f, 0.f));
		MeshComponent->AddImpulse(FVector(0.f, 0.f, 0.f));
	}
}

void ABasePawn::CreateBreakableMeshes()
{
	for (int32 i = 0; i < NumBreakableMeshes; ++i)
	{
		UStaticMeshComponent* NewMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(*FString::Printf(TEXT("BreakableMeshComponent%d"), i));
		BreakableMeshComponentCollection.Add(NewMeshComponent);
		NewMeshComponent->SetupAttachment(BaseMesh);
	}
}

void ABasePawn::HandleDestruction()
{
	//FallApart();

	CapsuleComp->Deactivate();

	if (DeathParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticles, GetActorLocation(), GetActorRotation());
	}
	if (DeathParticlesNiagaraSystem)
	{
			UNiagaraComponent* NewEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				this,
				DeathParticlesNiagaraSystem,
				GetActorLocation(),
				GetActorRotation(),
				FVector(1),
				true,
				true,
				ENCPoolMethod::AutoRelease,
				true);
		
		//DeathParticlesNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DeathParticlesNiagaraSystem, GetActorLocation(), GetActorRotation());
		//DeathParticlesNiagaraComponent->ActivateSystem();
	}
	if (DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	}
	if (DeathCameraShakeClass)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DeathCameraShakeClass);
	}
	Destroy();
}

void ABasePawn::Fire()
{

	FVector Location = ProjectileSpawnPoint->GetComponentLocation();
	FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();

	//UE_LOG(LogTemp, Warning, TEXT("Proj Spawn Point in Fire %s"), *Location.ToString());

	
	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
		MainProjectileClass,
		Location,
		Rotation
	);

	Projectile->SetOwner(this);
	
	UGameplayStatics::PlaySoundAtLocation(this, PrimaryFireSound, GetActorLocation());
}

void ABasePawn::RotateTurret(FVector LookAtTarget)
{
	FVector ToTarget = LookAtTarget - TurretSceneComponent->GetComponentLocation();
	FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);

    UE_LOG(LogTemp, Warning, TEXT("Rotate Turret: %s"), *LookAtRotation.ToString());

	//TurretSceneComponent->SetWorldRotation(LookAtRotation);

	TurretSceneComponent->SetWorldRotation(
		FMath::RInterpTo(
			TurretSceneComponent->GetComponentRotation(),
			LookAtRotation,
			UGameplayStatics::GetWorldDeltaSeconds(this),
			TurretRotationRate));

	// Rotate mesh

	/*FVector ToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
	FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);*/
	/*if (TurretMesh)
	{
		TurretMesh->SetWorldRotation(
			FMath::RInterpTo(
				TurretMesh->GetComponentRotation(),
				LookAtRotation,
				UGameplayStatics::GetWorldDeltaSeconds(this),
				TurretRotationRate));
	}*/
}

//void ABasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//}

