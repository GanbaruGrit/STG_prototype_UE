// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUp.h"
#include "PlayerCharacter.h"


// Sets default values
APowerUp::APowerUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
    SetRootComponent(CapsuleComp);
	CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &APowerUp::OnOverlapBegin);


    BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
    BaseMesh->SetupAttachment(CapsuleComp);






}

// Called when the game starts or when spawned
void APowerUp::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void APowerUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APowerUp::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("PowerUp Overlap hit"));
		APlayerCharacter* PlayerCharacterPtr = Cast<APlayerCharacter>(OtherActor);
		if (PlayerCharacterPtr != nullptr)
		{
			PlayerCharacterPtr->SetPowerLevel();
			Destroy();
		}
	}
}

