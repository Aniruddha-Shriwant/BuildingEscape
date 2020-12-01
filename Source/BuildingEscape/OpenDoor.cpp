// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Components/AudioComponent.h"
#include "Engine/TriggerVolume.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialZ = 0.0f;

	OpenSound = true;
	CloseSound = false;

	FindPressurePlate();
	FindAudioComponent();

}

void UOpenDoor::FindPressurePlate()
{
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s has OpenDoor Component but no PressurePlate attached!"), *GetOwner()->GetName());
	}
}

void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();

	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Audio component not found on %s"), *GetOwner()->GetName());
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PressurePlate) { return; }
	if (PressurePlate->IsOverlappingActor(Real) || PressurePlate->IsOverlappingActor(Fake))
	{
		CloseSound = false;
		OpenDoor(DeltaTime);	
	}
	else
	{
		OpenSound = false;
		CloseDoor(DeltaTime);
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	FVector Location = GetOwner()->GetActorLocation();
	TargetZ = InitialZ;
	CurrentZ = Location.Z;
	CurrentZ = FMath::FInterpTo(CurrentZ, TargetZ, DeltaTime, DoorOpenSpeed);
	Location.Z = CurrentZ;
	GetOwner()->SetActorLocation(Location, false, nullptr, ETeleportType::None);
	if (!OpenSound)
	{
			AudioComponent->Play();
			OpenSound = true;
	}
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	FVector Location = GetOwner()->GetActorLocation();
	TargetZ = -285.0f;
	CurrentZ = Location.Z;
	CurrentZ = FMath::FInterpTo(CurrentZ, TargetZ, DeltaTime, DoorCloseSpeed);
	Location.Z = CurrentZ;
	GetOwner()->SetActorLocation(Location, false, nullptr, ETeleportType::None);
	if (!CloseSound)
		{
			AudioComponent->Play();
			CloseSound = true;
		}
}


