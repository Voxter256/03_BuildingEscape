// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "Grabber.h"

#define OUT // does nothing, just for reference

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FirstPlayer = GetWorld()->GetFirstPlayerController();

	FindPhysicsComponent();
	SetupInputComponent();
}

//Look for attached Physics Component
void UGrabber::FindPhysicsComponent() {
	///Look for attache Physics Handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("%s is missing a UPhysicsHandleComponent"), *GetOwner()->GetName())
	}
}

//Look for attached Input Component
void UGrabber::SetupInputComponent() {
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s is missing a UInputComponent"), *GetOwner()->GetName())
	}
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (!PhysicsHandle) { return; }
	// if the physics handle is attached
	if (PhysicsHandle->GrabbedComponent) {
		// move the object we're holding
		FVector LineTraceEnd = GetPlayerReachEnd();

		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
		
	
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"))

	// Try (Line Trace) and reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	// If we hit something then attach a physics handle
	if (ActorHit) {
		if (!PhysicsHandle) { return; }
		PhysicsHandle->GrabComponent(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), true);
	}
	
}

void UGrabber::Release(){
	if (!PhysicsHandle) { return; }
	// Release physics
	PhysicsHandle->ReleaseComponent();
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach() {
	/// Setup query parameters
	FCollisionQueryParams TraceParameters = FCollisionQueryParams(FName(TEXT("")), false, GetOwner());

	/// Line-trace (AKA ray-cast) out to reach distance
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByObjectType(OUT HitResult, GetPlayerReachStart() , GetPlayerReachEnd(), FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParameters);
	
	return HitResult;
}

FVector UGrabber::GetPlayerReachStart() {
	FVector PlayerViewpointLocation;
	FRotator PlayerRotation;
	FirstPlayer->GetPlayerViewPoint(OUT PlayerViewpointLocation, OUT PlayerRotation);

	return PlayerViewpointLocation;
}

FVector UGrabber::GetPlayerReachEnd() {
	FVector PlayerViewpointLocation;
	FRotator PlayerRotation;
	FirstPlayer->GetPlayerViewPoint(OUT PlayerViewpointLocation, OUT PlayerRotation);
	
	return PlayerViewpointLocation + PlayerRotation.Vector() * Reach;
}
