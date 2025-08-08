
// Fill out your copyright notice in the Description page of Project Settings.


#include "MaterialInstanceDynamicChange.h"

// Sets default values
AMaterialInstanceDynamicChange::AMaterialInstanceDynamicChange()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);

}

// Called when the game starts or when spawned
void AMaterialInstanceDynamicChange::BeginPlay()
{
	Super::BeginPlay();

	// Get Material in first slot (needs to be material instance)
	Material = Mesh->GetMaterial(0);

	// Make a new Dynamic Material Instance from the Material stored above
	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);

	// Set the Material slot 0 to be our Dynamic version. This allows runtime changes.
	Mesh->SetMaterial(0, DynamicMaterial);

	// Set the parameters for the Dynamic Material Instance, these will be the names inside of the Material/Material Instance.
	DynamicMaterial->SetScalarParameterValue(TEXT("EmissiveStrength"), 50);
	DynamicMaterial->SetVectorParameterValue(TEXT("Color"), FLinearColor::Red);
	
}
