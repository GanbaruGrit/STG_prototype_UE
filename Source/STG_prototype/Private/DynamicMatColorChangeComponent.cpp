// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicMatColorChangeComponent.h"

// Sets default values for this component's properties
UDynamicMatColorChangeComponent::UDynamicMatColorChangeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	/*Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetupAttachment(RootComponent);*/
}


// Called when the game starts
void UDynamicMatColorChangeComponent::BeginPlay()
{
	Super::BeginPlay();

	// Get Material in first slot (needs to be material instance)
	//Material = Mesh->GetMaterial(0);

	//// Make a new Dynamic Material Instance from the Material stored above
	//UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);

	//// Set the Material slot 0 to be our Dynamic version. This allows runtime changes.
	//Mesh->SetMaterial(0, DynamicMaterial);

	//// Set the parameters for the Dynamic Material Instance, these will be the names inside of the Material/Material Instance.
	//DynamicMaterial->SetScalarParameterValue(TEXT("EmissiveStrength"), 50);
	//DynamicMaterial->SetVectorParameterValue(TEXT("Color"), FLinearColor::Red);
	
}


// Called every frame
void UDynamicMatColorChangeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

