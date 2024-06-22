// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/GrabberComponent.h"
#include "Engine/World.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

// Sets default values for this component's properties
UGrabberComponent::UGrabberComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//PrimaryComponentTick.bCanEverTick = true;

	// ...

	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabberComponent::BeginPlay()
{
	Super::BeginPlay();	
}


// Called every frame
void UGrabberComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (IsValid(PhysicsHandle) == false)
	{
		return;
	}

	FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
	PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
}

UPhysicsHandleComponent* UGrabberComponent::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (IsValid(PhysicsHandle) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("Grabber requires a UPhyiscsHandleComponent"));
		ensure(false);
	}

	return PhysicsHandle;
}

void UGrabberComponent::BP_Grab()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (IsValid(PhysicsHandle) == false)
	{
		return;
	}

	FVector StartLocation = GetComponentLocation();
	FVector EndLocation = StartLocation + GetForwardVector() * MaxGrabDistance;
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red);
	DrawDebugSphere(GetWorld(), EndLocation, 10.f, 10, FColor::Blue, false, 5.f);

	FCollisionShape Shape = FCollisionShape::MakeSphere(GrabRadius);
	FHitResult HitResult;
	bool bHasHit = GetWorld()->SweepSingleByChannel
	(
		HitResult,
		StartLocation,
		EndLocation,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		Shape
	);

	if (bHasHit == true)
	{
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			HitResult.GetComponent(),
			NAME_None,
			HitResult.ImpactPoint,
			GetComponentRotation()
		);
	}
}

void UGrabberComponent::BP_Release()
{

}

