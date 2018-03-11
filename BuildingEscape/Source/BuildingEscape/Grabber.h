// Copyright Mathew Connor Jennings 2018.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Engine/EngineTypes.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"
#include "DrawDebugHelpers.h"
#include "Grabber.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(GrabberLog, Log, All);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();
    
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
    // How far ahead of the player we can reach in cm
    float Reach = 100.0f;
    
    UPhysicsHandleComponent* PhysicsHandle = nullptr;
    UInputComponent* InputComponent = nullptr;
    
    // Find attached Physics Handle
    void FindPhysicsHandleComponent();
    
    // Setup Input Component
    void SetupInputComponent();
    
    // Ray-cast and grab what is in reach
    void Grab();
    
    // Call when Grab key is released
    void Release();
    
    // Return hit for first physics body in reach
    FHitResult GetFirstPhysicsBodyInReach() const;
};
