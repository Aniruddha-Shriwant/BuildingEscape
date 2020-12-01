// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Grabber.generated.h"

class UPhysicsHandleComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// Called when pressed right mouse button
	void Press();
	// Called when released right mouse button
	void Release();
	// Finds the physics handle component
	void FindPhysicsHandle();
	// Finds and set the input component
	void FindAndSetInput();
	// Gets the first actor that was hit using ray-cast
	FHitResult GetFirstHit() const;
	// Sets the LineTraceEnd
	void SetLineTraceEnd();
	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	float Reach = 150.0f; // Specifies how far the rays are casted

	FVector Location; // Location of the player
	FRotator Rotation; // Rotation of the player

	FVector LineTraceEnd; // End of the ray

	UPROPERTY()
	UPhysicsHandleComponent* PhysicsHandle = nullptr; // Pointer to the PhysicsHandle component
	
	UPROPERTY()
	UInputComponent* InputComponent = nullptr; // Pointer to the Input component
};
