// Copyright ScottLivingstone 2018.

#include "Graber.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "Components/PrimitiveComponent.h"
#include "Color.h"


#define OUT

// Sets default values for this component's properties
UGraber::UGraber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGraber::BeginPlay()
{
	Super::BeginPlay();
	Controller = World->GetFirstPlayerController();
	
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

/// Look for attached Physics Handle
void UGraber::FindPhysicsHandleComponent()
{
	PhysicsHandle = Owner->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle != NULL)
	{
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UPhysicsHAndleComponent is NOT attached to %s"), *Owner->GetName());
	}
}

/// Look for attached Input Component (only appears at run-time)
void UGraber::SetupInputComponent()
{
	InputComponent = Owner->FindComponentByClass<UInputComponent>();
	if (InputComponent != NULL)
	{
		/// Bind the input action
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGraber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGraber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UInputComponent is NOT attached to %s"), *Owner->GetName());
	}
}

void UGraber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed!"));

	/// Try and reach any actors with phsics body collision channel set
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	AActor* ActorHit = HitResult.GetActor();

	/// If we hit something then attach physics handle
	if (ActorHit)
	{
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true // allow rotation
		);
	}
	
}

void UGraber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab released!"));
	PhysicsHandle->ReleaseComponent();
}

UGraber::LineTrace UGraber::GetLineTrace()
{
	LineTrace CurrentLineTrace;

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	/// Sets the player location to the above variables.  The function does NOT return anything.
	Controller->GetPlayerViewPoint(OUT CurrentLineTrace.PlayerLocation, OUT CurrentLineTrace.PlayerRotation);

	CurrentLineTrace.End = CurrentLineTrace.PlayerLocation + (CurrentLineTrace.PlayerRotation.Vector() * Reach);

	return CurrentLineTrace;
}

const FHitResult UGraber::GetFirstPhysicsBodyInReach()
{
	/// Setup query params
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	/// Get line-trace vectors
	UGraber::LineTrace LineTrace = GetLineTrace();

	/// Line-trace (AKA ray-cast)
	FHitResult Hit;
	World->LineTraceSingleByObjectType(
		OUT Hit,
		LineTrace.PlayerLocation,
		LineTrace.End,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	/// See What we hit
	if (Hit.Actor != NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor hit: %s"), *Hit.Actor->GetName());
	}

	return Hit;
}

// Called every frame
void UGraber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if the physics handle is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetLineTrace().End);
	}
		// move the object

}

