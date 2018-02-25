// Copyright ScottLivingstone 2018.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"
#include "Graber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGraber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGraber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	AActor* Owner = GetOwner();
	APlayerController* Controller;
	UWorld* World = GetWorld();

	// How far the player can reacy
	UPROPERTY(EditAnywhere)
		float Reach = 100.f;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	// Find attached physics handle
	void FindPhysicsHandleComponent();

	// Setup attached input component
	void SetupInputComponent();

	// Return the first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();

	// Line-Trace and grab what's in reach
	void Grab();

	// Release held item.  Called when grab key is released
	void Release();
};