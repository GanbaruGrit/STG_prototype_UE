
#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "STGGameMode.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::DamageTaken);

	STGGameMode = Cast<ASTGGameMode>(UGameplayStatics::GetGameMode(this)); // Storing game mode
}


void UHealthComponent::DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* Instigator, AActor* DamageCauser)
{
	if (Damage <= 0.f) return;


	Health -= Damage;

	if (Health <= 0.f && STGGameMode)
	{
		STGGameMode->ActorDied(DamagedActor);
	}

	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), Health);
}

// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

