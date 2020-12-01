// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
//#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabber::FindPhysicsHandle()
{
	// Finds and sets the PhysicsHandle component.
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	// Throws an error if component not found.
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("No physics handle component present on %s"), *GetOwner()->GetName());
	}
}

void UGrabber::FindAndSetInput()
{
	// Finds and sets the Input component.
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent)
	{
		// Binds action to do with the keys pressed.
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Press);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		// Throws an error if component not found.
		UE_LOG(LogTemp, Error, TEXT("No input component present on %s"), *GetOwner()->GetName());
	}
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	FindAndSetInput();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	SetLineTraceEnd();

	// If the player is grabbing an object, sets updates the target location every frame.
	if (!PhysicsHandle) { return; }
	if (PhysicsHandle->GrabbedComponent) 
	{
		// Target location is LineTraceEnd.
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

FHitResult UGrabber::GetFirstHit() const
{
	// Debug DrawDebugLine(GetWorld(), Location, LineTraceEnd, FColor(100.0f, 0.0f, 150.0f), false, 0.0f, 0, 5.0f);

	FHitResult Hit;

	// Decides which objects we want to hit.
	FCollisionObjectQueryParams TraceObjectParams = { ECollisionChannel::ECC_PhysicsBody };

	// Specifies the collision criteria.
	FCollisionQueryParams TraceParams = { FName(TEXT("")), false, GetOwner() };

	// Traces the object.
	GetWorld()->LineTraceSingleByObjectType(OUT Hit, Location, LineTraceEnd, TraceObjectParams, TraceParams);

	// Gets the actor that was hit
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		// Logs the actor name.
		UE_LOG(LogTemp, Warning, TEXT("%s"), *ActorHit->GetName());
	}
	
	return Hit;
}

void UGrabber::SetLineTraceEnd()
{
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT Location, OUT Rotation);
	LineTraceEnd = Location + Rotation.Vector() * Reach;
}

void UGrabber::Press()
{
	UE_LOG(LogTemp, Warning, TEXT("Key pressed."));
	
	SetLineTraceEnd();

	FHitResult HitResult = GetFirstHit();
	
	if (HitResult.GetActor())
	{
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();

		if (!PhysicsHandle) { return; }
		PhysicsHandle->GrabComponentAtLocation(HitComponent, NAME_None, LineTraceEnd);
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Key released."));
	
	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent();
}

