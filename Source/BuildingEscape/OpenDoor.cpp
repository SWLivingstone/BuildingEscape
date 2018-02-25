// Copyright ScottLivingstone 2018.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"


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
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void UOpenDoor::OpenDoor()
{
	Door->SetActorRotation(FRotator(0.f, -75.f, 0.f));
}

void UOpenDoor::CloseDoor()
{
	Door->SetActorRotation(FRotator(0.f, 0.f, 0.f));
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the Trigger Volume every frame
	if (PressurePlate->IsOverlappingActor(ActorThatOpens))
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

