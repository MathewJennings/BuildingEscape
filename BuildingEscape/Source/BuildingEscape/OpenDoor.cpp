// Copyright Mathew Connor Jennings 2018.

#include "OpenDoor.h"

#define OUT

DEFINE_LOG_CATEGORY(OpenDoorLog);

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
    Owner = GetOwner();
    UOpenDoor::FindPressurePlate();
}

void UOpenDoor::FindPressurePlate()
{
    if (!PressurePlate)
    {
        UE_LOG(OpenDoorLog, Error, TEXT("%s is missing a Pressure Plate TriggerVolume!"), *(GetOwner()->GetName()));
    }
}

void UOpenDoor::OpenDoor()
{
    Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
}

void UOpenDoor::CloseDoor()
{
    Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// Poll the trigger volume every frame
	if (GetTotalMassOfActorsOnPlate() > 30.0f) // TODO make into a parameter
	{
        UOpenDoor::OpenDoor();
        LastDoorOpenTime = GetWorld()->GetTimeSeconds();
    }
    
    // Check if we should close the door
    if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
    {
        UOpenDoor::CloseDoor();
    }
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.0f;

	TArray<AActor*> OverlappingActors;
    if (!PressurePlate) { return 0.0f; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	for (const auto* OverlappingActor : OverlappingActors)
	{
		TotalMass += OverlappingActor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}

