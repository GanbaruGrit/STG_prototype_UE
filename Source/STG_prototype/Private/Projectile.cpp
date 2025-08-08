// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "ReactiveSphere.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "Particles/ParticleSystem.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	SetRootComponent(CapsuleComp);
	
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	ProjectileMesh->SetupAttachment(CapsuleComp);

	
	NiagaraParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara System Component"));
	NiagaraParticles->SetupAttachment(ProjectileMesh);

	CoreProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	CoreProjectileMovementComponent->MaxSpeed = MaxSpeed;
	CoreProjectileMovementComponent->InitialSpeed = InitialSpeed;
	


	
	SetLifeSpan(6);
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	CapsuleComp->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	if (LaunchSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
	}

	

	if (CascadeParticles)
	{
		UParticleSystemComponent* ParticleSystemComponent = NewObject<UParticleSystemComponent>(this, NAME_None, RF_Transient);

		if (ParticleSystemComponent)
		{
			ParticleSystemComponent->AttachToComponent(this->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
			ParticleSystemComponent->SetTemplate(CascadeParticles);
		}
		
		//UGameplayStatics::SpawnEmitterAtLocation(this, CascadeParticles, GetActorLocation(), GetActorRotation());
		
	}
	
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}


void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//UE_LOG(LogTemp, Warning, TEXT("Hit Reg Early"));
	
	AActor* MyOwner = GetOwner();
	if (MyOwner == nullptr)
	{
		Destroy();
		return;
	}

	AController* MyOwnerInstigator = MyOwner->GetInstigatorController();

	UClass* DamageTypeClass = UDamageType::StaticClass();



	if (OtherActor && OtherActor != this && OtherActor != MyOwner)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Hit Reg Past Owner/Other check"));

		if (OtherActor->ActorHasTag(TEXT("Reactive Sphere")) && this->ActorHasTag(TEXT("Charge Shot")))
		{
			//UE_LOG(LogTemp, Warning, TEXT("Reactive Sphere Tag Reg!"));
			//if (ReactiveSphere2nd)
			//{
			//	AReactiveSphere* ReactiveExplosion2nd = GetWorld()->SpawnActor<AReactiveSphere>(
			//		ReactiveSphere2nd,
			//		OtherActor->GetActorLocation(),
			//		OtherActor->GetActorRotation()
			//	);
			//	//ReactiveExplosion->SetOwner(this);
			//}
		}

		//OtherComp->AddImpulseAtLocation(GetVelocity() * ImpulseForce, GetActorLocation());

		FVector Scale = OtherComp->GetComponentScale();
		Scale *= 0.8f;

		//OtherComp->SetWorldScale3D(Scale);

		//UMaterialInstanceDynamic* MatInst = OtherComp->CreateAndSetMaterialInstanceDynamic(0);
		//if (MatInst)
		//{
		//	//MatInst->SetVectorParameterValue("Color", FLinearColor::MakeRandomColor());
		//}
		
		UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwnerInstigator, this, DamageTypeClass);
		
		if (ReactiveSphere)
		{
			AReactiveSphere* ReactiveExplosion = GetWorld()->SpawnActor<AReactiveSphere>(
				ReactiveSphere,
				GetActorLocation(),
				GetActorRotation()
			);
			//ReactiveExplosion->SetOwner(this);
		}
		if (HitParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, GetActorLocation(), AltRotation);
		}
		if (HitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
		}
		if (HitCameraShakeClass)
		{
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShakeClass);
		}
		Destroy();
	}
}


