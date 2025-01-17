// Copyright Mathew Connor Jennings 2018.

#pragma once

#include <Engine/TriggerVolume.h>
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Gameframework/Actor.h>
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorEvent);
DECLARE_LOG_CATEGORY_EXTERN(OpenDoorLog, Log, All);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    
    UPROPERTY(BlueprintAssignable)
    FDoorEvent OnOpen;
    
    UPROPERTY(BlueprintAssignable)
    FDoorEvent OnClose;

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
