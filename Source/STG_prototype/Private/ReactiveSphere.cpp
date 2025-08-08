// Fill out your copyright notice in the Description page of Project Settings.


#include "ReactiveSphere.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "Particles/ParticleSystem.h"

// Sets default values
AReactiveSphere::AReactiveSphere()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	SetRootComponent(BaseMesh);

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
	SphereComp->SetCollisionProfileName(TEXT("Trigger"));
	SphereComp->SetupAttachment(BaseMesh);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AReactiveSphere::OnOverlapBegin);
	SphereComp->OnComponentEndOverlap.AddDynamic(this, &AReactiveSphere::OnOverlapEnd);

	NiagaraParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara System Component"));
	NiagaraParticles->SetupAttachment(BaseMesh);

	SetLifeSpan(TimeToLive);

	Tags.Add(FName("Reactive Sphere"));

	StartScale = 1.f;
	TargetScale = 2.f;
	CurrentTime = 0.f;
	TotalTime = 5.f;

}

// Called when the game starts or when spawned
void AReactiveSphere::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AReactiveSphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentTime += DeltaTime;

	ScaleMeshOverTime(bCanScale);

	if (CurrentTime >= TotalTime)
	{

	}

}

//void AReactiveSphere::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
//{
//	//UE_LOG(LogTemp, Warning, TEXT("Reactive Sphere Hit Reg"));
//	
//	AActor* MyOwner = GetOwner();
//	if (MyOwner == nullptr)
//	{
//		Destroy();
//		return;
//	}
//
//	AController* MyOwnerInstigator = MyOwner->GetInstigatorController();
//
//	UClass* DamageTypeClass = UDamageType::StaticClass();
//
//	if (OtherActor && OtherActor != this)
//	{
//
//		//OtherComp->AddImpulseAtLocation(GetVelocity() * ImpulseForce, GetActorLocation());
//
//		FVector Scale = OtherComp->GetComponentScale();
//		Scale *= 0.8f;
//
//		//OtherComp->SetWorldScale3D(Scale);
//
//		//UMaterialInstanceDynamic* MatInst = OtherComp->CreateAndSetMaterialInstanceDynamic(0);
//		//if (MatInst)
//		//{
//		//	//MatInst->SetVectorParameterValue("Color", FLinearColor::MakeRandomColor());
//		//}
//
//		//UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwnerInstigator, this, DamageTypeClass);
//
//		if (OtherActor->ActorHasTag(TEXT("Reactive Sphere")))
//		{
//			UE_LOG(LogTemp, Warning, TEXT("CHAIN SPHERE HIT"));
//			
//			if (CascadeParticles)
//			{
//				UGameplayStatics::SpawnEmitterAtLocation(this, CascadeParticles, GetActorLocation(), GetActorRotation());
//			}
//		}
//
//		if (HitSound)
//		{
//			UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
//		}
//
//		if (HitCameraShakeClass)
//		{
//			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShakeClass);
//		}
//
//		//Destroy();
//	}
//}

void AReactiveSphere::ScaleMeshOverTime(bool bScale)
{
	if (bCanScale)
	{
		float NewScale = FMath::Lerp(StartScale, TargetScale, (CurrentTime / TotalTime) * ScaleRate);
		//UE_LOG(LogTemp, Warning, TEXT("NewScale: %d"), NewScale);

		BaseMesh->SetRelativeScale3D(FVector(NewScale, NewScale, NewScale));
		SphereComp->SetRelativeScale3D(FVector(NewScale, NewScale, NewScale));
	}
}

void AReactiveSphere::DestroySelf()
{
	Destroy();
}

void AReactiveSphere::BeginScaling()
{
	bCanScale = true;
}

void AReactiveSphere::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp && OtherActor->ActorHasTag(TEXT("Triggered By Charge Shot")) && OtherActor->ActorHasTag(TEXT("Reactive Sphere")))
	{
		GetWorld()->GetTimerManager().SetTimer(DelayScaleTimerHandle, this, &AReactiveSphere::BeginScaling, ScaleDelay, false);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap Begin"));
		
			//UE_LOG(LogTemp, Warning, TEXT("CHAIN SPHERE HIT"));
						
			if (CascadeParticles)
				{
					UGameplayStatics::SpawnEmitterAtLocation(this, CascadeParticles, GetActorLocation(), GetActorRotation());
				}
			if (BP_Effect)
			{
				UGameplayStatics::SpawnObject(BP_Effect, this);
			}
			GetWorld()->GetTimerManager().SetTimer(DelayDestroyTimerHandle, this, &AReactiveSphere::DestroySelf, DestroyDelay, false);
			//Destroy();
	}

	if (OtherActor && (OtherActor != this) && OtherComp && OtherActor->ActorHasTag(TEXT("Charge Shot")))
	{
		bCanScale = true;
		Tags.Add(FName("Triggered By Charge Shot"));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap Begin"));

		//UE_LOG(LogTemp, Warning, TEXT("CHAIN SPHERE HIT"));

		if (CascadeParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(this, CascadeParticles, GetActorLocation(), GetActorRotation());
		}
		if (BP_Effect)
		{
			UGameplayStatics::SpawnObject(BP_Effect, this);
		}
		GetWorld()->GetTimerManager().SetTimer(DelayDestroyTimerHandle, this, &AReactiveSphere::DestroySelf, DestroyDelay, false);
		//Destroy();
	}
}

void AReactiveSphere::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap End"));
	}
}

