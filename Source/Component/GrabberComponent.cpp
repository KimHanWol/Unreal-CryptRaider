// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/GrabberComponent.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UGrabberComponent::UGrabberComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabberComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGrabberComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector StartLocation = GetComponentLocation();
	FVector EndLocation = StartLocation + GetForwardVector() * MaxGrabDistance;
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red);

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
		AActor* HitActor = HitResult.GetActor();
		UE_LOG(LogTemp, Display, TEXT("Hit Actor: %s"), *HitActor->GetActorNameOrLabel());
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("No Actor Hit"));
	}

}

