// Copyright Mathew Connor Jennings 2018.

#pragma once

#include <Engine/TriggerVolume.h>
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Gameframework/Actor.h>
#include "MoveObject.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMoveEvent);
DECLARE_LOG_CATEGORY_EXTERN(MoveObjectLog, Log, All);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BUILDINGESCAPE_API UMoveObject : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMoveObject();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable)
		FMoveEvent OnMoveAway;

	UPROPERTY(BlueprintAssignable)
		FMoveEvent OnMoveBack;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
	float TriggerMass = 30.0f;

	UPROPERTY(EditAnywhere)
	FName QualifyingActorTag = FName(TEXT(""));

	// The owning door
	AActor* Owner = nullptr;

	// Find attached Pressure Plate Trigger Volume
	void FindPressurePlate();

	// Returns total mass in kg
	float GetTotalMassOfQualifyingActorsOnPlate();
};