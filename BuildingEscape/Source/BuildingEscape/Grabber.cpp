// Copyright Mathew Connor Jennings 2018.

#include "Grabber.h"

#define OUT

DEFINE_LOG_CATEGORY(GrabberLog);

UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabber::BeginPlay()
{
	Super::BeginPlay();
    FindPhysicsHandleComponent();
    SetupInputComponent();
}

void UGrabber::FindPhysicsHandleComponent()
{
    PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
    if (PhysicsHandle == nullptr)
    {
        UE_LOG(GrabberLog, Error, TEXT("%s is missing a UPhysicsHandleComponent!"), *(GetOwner()->GetName()));
    }
}

void UGrabber::SetupInputComponent()
{
    InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
    if (InputComponent)
    {
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
    auto HitResult = GetFirstPhysicsBodyInReach();
    auto ComponentToGrab = HitResult.GetComponent(); // Gets the mesh in this case
    auto ActorHit = HitResult.GetActor();
    if (ActorHit)
    {
        PhysicsHandle->GrabComponentAtLocationWithRotation(
            ComponentToGrab,
            NAME_None, // No bones needed.
            ComponentToGrab->GetOwner()->GetActorLocation(),
            FRotator::ZeroRotator // Allow rotation
        );
    }
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
    /// Ray-cast out to reach distance.
    FHitResult HitResult;
    FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
    GetWorld()->LineTraceSingleByObjectType(OUT HitResult, GetReachLineStart(), GetReachLineEnd(), FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParams);
    return HitResult;
}

void UGrabber::Release()
{
    PhysicsHandle->ReleaseComponent();
}

void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if (!PhysicsHandle) { return; }
    if (PhysicsHandle->GrabbedComponent)
    {
        PhysicsHandle->SetTargetLocation(GetReachLineEnd());
    }
}

FVector UGrabber::GetReachLineStart() const
{
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
    return PlayerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd() const
{
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
    return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}
