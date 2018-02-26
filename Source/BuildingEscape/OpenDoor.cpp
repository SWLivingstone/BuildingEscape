// Copyright ScottLivingstone 2018.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Containers/Array.h"
#include "Components/PrimitiveComponent.h"
#define OUT


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
	Door = GetOwner(); // Get Object pointer
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("PressurePlate not set on %s"), *Door->GetName())
	}
}

void UOpenDoor::OpenDoor()
{
	Door->SetActorRotation(FRotator(0.f, -75.f, 0.f));
}

void UOpenDoor::CloseDoor()
{
	Door->SetActorRotation(FRotator(0.f, 0.f, 0.f));
}

float UOpenDoor::GetTotalMassOfActorsOnPlayer()
{
	float TotalMass = 0.f;

	TArray<AActor*> OverLappingActors;
	if (!PressurePlate) { return 0.f; };
	PressurePlate->GetOverlappingActors(OUT OverLappingActors);

	for (AActor* Actor : OverLappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the Trigger Volume every frame
	if (GetTotalMassOfActorsOnPlayer() >= TriggerMass)
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	
	// Check if it is time to close the door

	if (LastDoorOpenTime + DoorCloseDelay <= GetWorld()->GetTimeSeconds())
	{
		CloseDoor();
	}
}

