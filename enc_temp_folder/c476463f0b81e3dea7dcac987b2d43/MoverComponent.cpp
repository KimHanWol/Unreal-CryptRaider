// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/MoverComponent.h"
#include "Math/UnrealMathUtility.h"

UMoverComponent::UMoverComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UMoverComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OriginalLocation = GetOwner()->GetActorLocation();
}

void UMoverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bShouldMove == true)
	{
		FVector CurrentLocation = GetOwner()->GetActorLocation();
		FVector TargetLocation = OriginalLocation + MoveOffset;
		float Speed = FVector::Distance(CurrentLocation, TargetLocation) / MoveTime;
		FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);
		GetOwner()->SetActorLocation(NewLocation);
	}
}

