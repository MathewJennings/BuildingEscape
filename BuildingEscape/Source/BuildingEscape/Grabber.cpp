// Copyright Mathew Connor Jennings 2018.

#include "Grabber.h"

#define OUT

DEFINE_LOG_CATEGORY(GrabberLog);

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(GrabberLog, Warning, TEXT("Grabber reporting for duty!"));
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    /// Get player viewpoint this tick.
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
        OUT PlayerViewPointLocation,
        OUT PlayerViewPointRotation
    );
    
    /// Draw a red trace in the world to visualize
    FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
    DrawDebugLine(GetWorld(), PlayerViewPointLocation, LineTraceEnd, FColor(255, 0, 0), false, 0.0f, 0.0f,  10.0f);

    /// Setup query params
    FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

    /// Ray-cast out to reach distance.
    FHitResult Hit;
    GetWorld()->LineTraceSingleByObjectType(OUT Hit, PlayerViewPointLocation, LineTraceEnd, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParams);

    /// See what we hit.
    AActor* HitActor = Hit.GetActor();
    if (HitActor)
    {
        FString HitActorName = HitActor->GetName();
        UE_LOG(GrabberLog, Warning, TEXT("Hit: %s"), *HitActorName);
    }
}

