// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"

#define OUT
#define DEBUG false
#define NOT !

FRotator DoorClosed;
FRotator DoorOpened = FRotator(0.f, 70.f, 0.f);

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	if (NOT PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: PressurePlate is <nullptr>"), *GetOwner()->GetName());
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (GetTotalMassOfActorsOnPlate() > TriggerMass)
	{
		OnOpen.Broadcast();
	}
	else
	{
		OnClose.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;
	// find all the overlapping actors
	TArray<AActor*> OverlappingActors;
	if (PressurePlate)
	{
		PressurePlate->GetOverlappingActors(OUT OverlappingActors);
		for (const auto* Actor : OverlappingActors)
		{
			TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		}
	}
	if (DEBUG) UE_LOG(LogTemp, Warning, TEXT("Weight calculated as %f"), TotalMass);
	return TotalMass;
}

