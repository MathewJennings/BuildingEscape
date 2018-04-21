// Copyright Mathew Connor Jennings 2018.

#include "MoveObject.h"

#define OUT

DEFINE_LOG_CATEGORY(MoveObjectLog);

// Sets default values for this component's properties
UMoveObject::UMoveObject()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMoveObject::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	UMoveObject::FindPressurePlate();
}

void UMoveObject::FindPressurePlate()
{
	if (!PressurePlate)
	{
		UE_LOG(MoveObjectLog, Error, TEXT("%s is missing a Pressure Plate TriggerVolume!"), *(GetOwner()->GetName()));
	}
}

// Called every frame
void UMoveObject::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the trigger volume every frame
	if (GetTotalMassOfQualifyingActorsOnPlate() > TriggerMass)
	{
		OnMoveAway.Broadcast();
	}
	else
	{
		OnMoveBack.Broadcast();
	}
}

float UMoveObject::GetTotalMassOfQualifyingActorsOnPlate()
{
	float TotalMass = 0.0f;

	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) { return 0.0f; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	for (const auto* OverlappingActor : OverlappingActors)
	{
		if (OverlappingActor->ActorHasTag(QualifyingActorTag))
		{
			TotalMass += OverlappingActor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		}
	}

	return TotalMass;
}

