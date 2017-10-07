// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "Components/PrimitiveComponent.h"

#define OUT
#define NOT !

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts,
	// and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	GetPhysicsHandle();
	GetInputComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetPlayerViewPoint());
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab received"));

	/// LINE TRACE and see if we reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	/// If we hit something then attach a physics handle
	// TODO attach physics handle
	if (ActorHit)
	{
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true
		);
	}

}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Release received"));
	PhysicsHandle->ReleaseComponent();
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle()
{
	// Look for attached Physics Handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		UE_LOG(LogTemp, Warning, TEXT("Physics handle found for %s\n"),
			*GetOwner()->GetName());
	} else {
		// Physics handle not found
		UE_LOG(LogTemp, Error, TEXT("Physics handle not found for %s\n"),
			*GetOwner()->GetName());
	}
	return PhysicsHandle;
}

UInputComponent * UGrabber::GetInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		// Bind input actions
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	} else {
		UE_LOG(LogTemp, Error, TEXT("Input Component MISSING for %s\n"),
			*GetOwner()->GetName());
	}
	return InputComponent;
}

FVector UGrabber::GetPlayerViewPoint()
{
	// Get Player View point
	// This is a "void" Get function which alters the arguments
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		// Macro OUT does nothing, but signals that these are parameters modified by a Get function
		OUT PlayerViewpointLocation,
		OUT PlayerViewpointRotation);
	// Log our viewpoint every tick
	//UE_LOG(LogTemp, Warning, TEXT("Location is %s, Rotation is %s\n"),
	//	*PlayerVector.ToString(), *PlayerRotator.ToString());

	/// Calculate end point of reach for ray-cast
	FVector LineTraceEnd = PlayerViewpointLocation + PlayerViewpointRotation.Vector() * Reach;

	/// Ray-cast out to reach distance
//	DrawDebugLine(GetWorld(), PlayerViewpointLocation, LineTraceEnd,
//		FColor(0, 255, 0), false, 0.f, 0.f, 10.f
//	);
	return LineTraceEnd;
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	/// Set up Query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	/// Line-trace (AKA ray-cast) out to reach distance
	HaveGotHit =
		GetWorld()->LineTraceSingleByObjectType(
			OUT Hit,
			PlayerViewpointLocation,
			GetPlayerViewPoint(),
			FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
			TraceParameters
		);
	/// Log what we hit, the first time we hit
	if (HaveGotHit)
	{
		/// This ensures we log overlapping hits
		if ( NOT Hit.Actor->GetName().Equals(ObjectTouched))
		{
			ObjectTouched = Hit.Actor->GetName();
			LogRequired = true;
		}
	}
	else
	{
		ObjectTouched = "";
	}
	if (LogRequired)
	{
		UE_LOG(LogTemp, Warning, TEXT("We hit a %s"), *ObjectTouched);
		LogRequired = false;
	}
	return Hit;
}
