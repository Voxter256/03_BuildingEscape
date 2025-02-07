// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

private:

	APlayerController* FirstPlayer;
	float Reach = 132.f;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	void FindPhysicsComponent(); //needs to be attached
	void SetupInputComponent(); //needs to be attached
	
	void Grab(); // Ray-cast and grab what's in reach
	void Release(); // Release Grabbed Object
	
	// Return hit for first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();
	// Returns current start of reach line
	FVector GetPlayerReachStart();
	// Returns current end of reach line
	FVector GetPlayerReachEnd();
};
