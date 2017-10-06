// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty\n"));

	// ...
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get Player View point this tick

	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;

	// This is a "void" Get function
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		// Macro OUT does nothing, but signals that these are parameters modified by a Get function
		OUT PlayerViewpointLocation,
		OUT PlayerViewpointRotation);

	// Log our viewpoint every tick
	//UE_LOG(LogTemp, Warning, TEXT("Location is %s, Rotation is %s\n"),
	//	*PlayerVector.ToString(), *PlayerRotator.ToString());

	FVector LineTraceEnd = PlayerViewpointLocation + PlayerViewpointRotation.Vector() * Reach;

	// Ray-cast out to reach distance
	DrawDebugLine(
		GetWorld(),
		PlayerViewpointLocation,
		LineTraceEnd,
		FColor(0,255,0),
		false,
		0.f,
		0.f,
		10.f
	);

	/// Set up Query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	/// Line-trace (AKA raay-cast) out to reach distance
	FHitResult Hit;
	bool GotHit =
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewpointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	// See what we hit
	if(GotHit)
		UE_LOG(LogTemp, Warning, TEXT("We hit a %s"), *Hit.Actor->GetName());
}

