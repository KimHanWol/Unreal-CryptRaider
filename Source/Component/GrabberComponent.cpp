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

	if (IsValid(PhysicsHandle->GetGrabbedComponent()) == false)
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

bool UGrabberComponent::GetGrabbableInReach(FHitResult& OutHitResult) const
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (IsValid(PhysicsHandle) == false)
	{
		return false;
	}

	FVector StartLocation = GetComponentLocation();
	FVector EndLocation = StartLocation + GetForwardVector() * MaxGrabDistance;
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red);
	DrawDebugSphere(GetWorld(), EndLocation, 10.f, 10, FColor::Blue, false, 5.f);

	FCollisionShape Shape = FCollisionShape::MakeSphere(GrabRadius);
	bool bHasHit = GetWorld()->SweepSingleByChannel
	(
		OutHitResult,
		StartLocation,
		EndLocation,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		Shape
	);

	return bHasHit;
}

void UGrabberComponent::BP_Grab()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (IsValid(PhysicsHandle) == false)
	{
		return;
	}

	FHitResult HitResult;
	bool HasHit = GetGrabbableInReach(HitResult);
	if (HasHit  == true)
	{
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		if (IsValid(HitComponent) == true)
		{
			HitComponent->WakeAllRigidBodies();
			PhysicsHandle->GrabComponentAtLocationWithRotation(
				HitComponent,
				NAME_None,
				HitResult.ImpactPoint,
				GetComponentRotation()
			);
		}
	}
}

void UGrabberComponent::BP_Release()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (IsValid(PhysicsHandle) == false)
	{
		return;
	}

	UPrimitiveComponent* GrabbedComponent = PhysicsHandle->GetGrabbedComponent();
	if (IsValid(GrabbedComponent) == true)
	{
		GrabbedComponent->WakeAllRigidBodies();
		PhysicsHandle->ReleaseComponent();		
	}
}

