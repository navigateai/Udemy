// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"

#define OUT
#define DEBUG true

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
	Owner = GetOwner();
}

void UOpenDoor::OpenDoor()
{
	Owner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
}

void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(FRotator(0.f, ClosedAngle, 0.f));
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (GetTotalMassOfActorsOnPlate() > 50.f)
	{
		OpenDoor();
	} else {
		CloseDoor();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;
	// find all the overlapping actors
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	if (OverlappingActors.Num() == 0)
		return 0.f;
	for (const auto* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("%s on pressure plate,  mass = %s, actor total mass: %s"),
			*Actor->GetName(),
			*FString::SanitizeFloat(Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass()),
			*FString::SanitizeFloat(TotalMass)
		);
	}
	if (DEBUG) UE_LOG(LogTemp, Warning, TEXT("Weight calculated as %f"), TotalMass);
	return TotalMass;
}

