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
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
    FindPhysicsHandleComponent();
    SetupInputComponent();
}

void UGrabber::FindPhysicsHandleComponent()
{
    PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
    if (!PhysicsHandle)
    {
        UE_LOG(GrabberLog, Error, TEXT("%s is missing a UPhysicsHandleComponent!"), *(GetOwner()->GetName()));
    }
}

void UGrabber::SetupInputComponent()
{
    InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
    if (InputComponent)
    {
        UE_LOG(GrabberLog, Warning, TEXT("UInputComponent found"), *(GetOwner()->GetName()));
        // Bind the input axis
        InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
        InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
    }
    else
    {
        UE_LOG(GrabberLog, Error, TEXT("%s is missing a UInputComponent!"), *(GetOwner()->GetName()));
    }
}

void UGrabber::Grab()
{
    UE_LOG(GrabberLog, Warning, TEXT("Grab key pressed"));
    // Line trace and see if we reach any actiors with physics body collision channel set
    FHitResult Hit = GetFirstPhysicsBodyInReach();
    // TODO attach physics handle
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
    /// Get player viewpoint.
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
    FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
    
    /// Ray-cast out to reach distance.
    FHitResult Hit;
    FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
    GetWorld()->LineTraceSingleByObjectType(OUT Hit, PlayerViewPointLocation, LineTraceEnd, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParams);
    
    /// See what we hit.
    AActor* HitActor = Hit.GetActor();
    if (HitActor)
    {
        FString HitActorName = HitActor->GetName();
        UE_LOG(GrabberLog, Warning, TEXT("Hit: %s"), *HitActorName);
    }
    return Hit;
}

void UGrabber::Release()
{
    UE_LOG(GrabberLog, Warning, TEXT("Grab key released"));
    // TODO release physics handle
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    // If the physics handle is attached
        // Move the object that we're holding
}
