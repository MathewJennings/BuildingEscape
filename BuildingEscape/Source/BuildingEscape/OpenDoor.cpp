// Copyright Mathew Connor Jennings 2018.

#include "OpenDoor.h"
#include <Gameframework/Actor.h>


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
    ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
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
    if (UOpenDoor::PressurePlate->IsOverlappingActor(UOpenDoor::ActorThatOpens))
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

