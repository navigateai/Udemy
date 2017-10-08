// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "Components/PrimitiveComponent.h"

#define OUT
#define NOT !
#define DEBUG false

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
	BindInputActions(GetInputComponent());
}
//--------------------------------------------------------------------------------------------------------------------
// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Relocate anything that has been grabbed by player
	if (GetPhysicsHandle()->GrabbedComponent)
	{
		GetPhysicsHandle()->SetTargetLocation(GetReachLineEnd());
	}
}
//--------------------------------------------------------------------------------------------------------------------
// Called when GRAB key pressed
void UGrabber::Grab()
{
	if(DEBUG) UE_LOG(LogTemp, Warning, TEXT("Grab received"));
	/// LINE TRACE and see if we reach any actors with physics body collision channel set
	/// If we hit something then attach a physics handle
	if (GetFirstPhysicsBodyInReach().GetActor())
	{
		GetPhysicsHandle()->GrabComponentAtLocation(
			GetFirstPhysicsBodyInReach().GetComponent(),
			NAME_None,
			GetFirstPhysicsBodyInReach().GetComponent()->GetOwner()->GetActorLocation()
		);
	}
}

//--------------------------------------------------------------------------------------------------------------------
// Called when GRAB key released
void UGrabber::Release()
{
	if(DEBUG) UE_LOG(LogTemp, Warning, TEXT("Release received"));
	GetPhysicsHandle()->ReleaseComponent();
}

//--------------------------------------------------------------------------------------------------------------------
// Returns current Owners Physics Handle Component
UPhysicsHandleComponent* UGrabber::GetPhysicsHandle()
{
	return GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
}

//--------------------------------------------------------------------------------------------------------------------
// Returns a structure containing location data for the player
// Refactors a C++ function that alters parameters to one returning the required info as a struct
UGrabber::PlayerViewPointStruct UGrabber::GetPlayerViewpointStructure()
{
	PlayerViewPointStruct PVP;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		// Macro OUT does nothing, but signals that these are parameters modified by a Get function
		OUT PVP.Location,
		OUT PVP.Rotation);
	return PVP;
}

//--------------------------------------------------------------------------------------------------------------------
// Returns current Owners Input Component
UInputComponent * UGrabber::GetInputComponent()
{
	return GetOwner()->FindComponentByClass<UInputComponent>();
}

//--------------------------------------------------------------------------------------------------------------------
// Returns the location in 3D space for the centre of the player
FVector UGrabber::GetReachLineStart()
{
	PlayerViewPointStruct PVP = GetPlayerViewpointStructure();
	return PVP.Location;
}

//--------------------------------------------------------------------------------------------------------------------
// Returns the location in 3D space for the centre of the player, including orientation, plus required reach
FVector UGrabber::GetReachLineEnd()
{
	PlayerViewPointStruct PVP = GetPlayerViewpointStructure();
	/// Calculate end point of reach for ray-cast
	return PVP.Location + PVP.Rotation.Vector() * Reach;
}

//--------------------------------------------------------------------------------------------------------------------
// Returns the first object hit (if any) for the current players reach
FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FHitResult HitResult;
	/// Set up Query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	/// Line-trace (AKA ray-cast) out to reach distance
	bool HaveGotHit = GetWorld()->LineTraceSingleByObjectType(
			OUT HitResult,
			GetReachLineStart(),
			GetReachLineEnd(),
			FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
			TraceParameters
		);
	if (DEBUG && HaveGotHit) UE_LOG(LogTemp, Warning, TEXT("We hit a %s"), *HitResult.GetActor()->GetName());
	return HitResult;
}

//--------------------------------------------------------------------------------------------------------------------
// Binds input actions to component methods. Actual keys and names are defined in Project
void UGrabber::BindInputActions(UInputComponent* InputComponent)
{
	if (InputComponent)
	{
		// "Grab" is already defined within unreal editor
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else if(DEBUG) UE_LOG(LogTemp, Error, TEXT("Input Component MISSING for %s\n"),*GetOwner()->GetName());
}
