// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveStatue.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Engine/TriggerVolume.h"
#include "GameFramework/PlayerController.h"
#include "Components/AudioComponent.h"

// Sets default values for this component's properties
UMoveStatue::UMoveStatue()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UMoveStatue::BeginPlay()
{
	Super::BeginPlay();
	Player = GetWorld()->GetFirstPlayerController()->GetPawn();
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	HasPlayed = false;

	FindPressurePlate();
	FindAudioComponent();
	// ...
	
}

void UMoveStatue::FindAudioComponent()
{
	Jumpscare = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!Jumpscare)
	{
		UE_LOG(LogTemp, Warning, TEXT("Audio component not attached."));
	}
}

void UMoveStatue::FindPressurePlate()
{
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s has OpenDoor Component but no PressurePlate attached!"), *GetOwner()->GetName());
	}
}

void UMoveStatue::MoveStatue()
{
	if (!HasPlayed)
	{
		Jumpscare->Play();
		HasPlayed = true;
	}
	TargetYaw = -90.0f;
	FRotator Rotation = GetOwner()->GetActorRotation();
	CurrentYaw = FMath::FInterpTo(CurrentYaw, TargetYaw, GetWorld()->DeltaTimeSeconds, MoveSpeed);
	Rotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(Rotation);
}


// Called every frame
void UMoveStatue::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!PressurePlate) { return; }
	if (PressurePlate->IsOverlappingActor(Player))
	{

		MoveStatue();
	}
	// ...
}

