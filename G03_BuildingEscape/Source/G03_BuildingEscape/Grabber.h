// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class G03_BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	const float					Reach = 100.f;

	struct PlayerViewPointStruct {
		FVector						Location;
		FRotator					Rotation;
	};
	FVector						PlayerViewpointLocation;
	FRotator					PlayerViewpointRotation;

	// Binds key actions to functions
	void						BindInputActions(UInputComponent*);
	// Returns first object (if any) within player's reach
	FHitResult					GetFirstPhysicsBodyInReach();
	// Returns Owner's Input Component
	UInputComponent*			GetInputComponent();
	// Returns Owner's Physics Handle
	UPhysicsHandleComponent*	GetPhysicsHandle();
	// Returns Structure containing Player Viewpoint information
	PlayerViewPointStruct		GetPlayerViewpointStructure();
	// Returns current player's reach start point
	FVector						GetReachLineStart();
	// Returns current player's reach end point
	FVector						GetReachLineEnd();
	// Grabs first object if one is within range
	void						Grab();
	// Releases current object, if any
	void						Release();
};
