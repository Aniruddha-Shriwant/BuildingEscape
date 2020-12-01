// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"

class UAudioComponent;
class ATriggerVolume;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	void FindPressurePlate();
	void FindAudioComponent();

private:
	float InitialZ;
	float CurrentZ;
	float TargetZ;

	bool OpenSound;
	bool CloseSound;

	UPROPERTY(EditAnywhere)
		float DoorOpenSpeed = 1.0f;

	UPROPERTY(EditAnywhere)
		float DoorCloseSpeed = 2.0f;

	UPROPERTY(EditAnywhere)
		ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
		AActor* Fake = nullptr;

	UPROPERTY(EditAnywhere)
		AActor* Real = nullptr;

	UPROPERTY()
		UAudioComponent* AudioComponent = nullptr;
};
