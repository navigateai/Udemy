// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

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
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	DoorClosed = GetOwner()->GetActorRotation();
}

void UOpenDoor::OpenDoor()
{
	GetOwner()->SetActorRotation(DoorOpened);
}
void UOpenDoor::CloseDoor()
{
	GetOwner()->SetActorRotation(DoorClosed);
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor();
	}
	else
	{
		CloseDoor();
	}
}

