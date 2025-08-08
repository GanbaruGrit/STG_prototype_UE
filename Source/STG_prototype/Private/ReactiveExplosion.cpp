// Fill out your copyright notice in the Description page of Project Settings.


#include "ReactiveExplosion.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"

// Sets default values
AReactiveExplosion::AReactiveExplosion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	SetRootComponent(BaseMesh);

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
	SphereComp->SetupAttachment(BaseMesh);

	NiagaraParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara System Component"));
	NiagaraParticles->SetupAttachment(BaseMesh);

	SetLifeSpan(6);
}

// Called when the game starts or when spawned
void AReactiveExplosion::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AReactiveExplosion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

