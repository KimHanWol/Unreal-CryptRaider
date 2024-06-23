// Fill out your copyright notice in the Description page of Project Settings.

#include "MoverComponent.h"
#include "TriggerComponent.h"

UTriggerComponent::UTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsValid(MoverComponent) == false)
	{
		return;
	}

	AActor* AcceptableActor = GetAcceptableActor();
	if (IsValid(AcceptableActor))
	{
		UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(AcceptableActor->GetRootComponent());
		if (IsValid(PrimitiveComponent))
		{
			PrimitiveComponent->SetSimulatePhysics(false);
		}
		AcceptableActor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
		MoverComponent->SetShouldMove(true);
	}
	else
	{
		MoverComponent->SetShouldMove(false);
	}
}

void UTriggerComponent::SetMover(UMoverComponent* InMoverComponent)
{
	MoverComponent = InMoverComponent;
}

AActor* UTriggerComponent::GetAcceptableActor() const
{
	TArray<AActor*> Actors;
	GetOverlappingActors(Actors);

	for (AActor* Actor : Actors)
	{
		bool bHasAcceptableTag = Actor->ActorHasTag(AcceptableActorTag);
		bool bIsGrabbed = Actor->ActorHasTag(TEXT("Grabbed"));
		if (bHasAcceptableTag == true && bIsGrabbed == false)
		{
			return Actor;
		}
	}

	return nullptr;
}
