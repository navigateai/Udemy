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
	Owner = GetOwner();
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
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
	if (PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
	{
		CloseDoor();
	}
		/*
	if (PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		if (! DoorIsOpen)
		{
			OpenDoor();
			DoorIsOpen = true;
			LastDoorOpenTime = GetWorld()->GetTimeSeconds();
		}
	}
	else
	{
		if (DoorIsOpen)
		{
			if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime >= DoorCloseDelay)
			{
				CloseDoor();
				DoorIsOpen = false;
			}
		}
	}
	*/
}

