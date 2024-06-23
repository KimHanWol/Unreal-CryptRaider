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

	AActor* AcceptableActor = GetAcceptableActor();
	if (IsValid(AcceptableActor))
	{
		UE_LOG(LogTemp, Display, TEXT("%s"), *AcceptableActor->GetActorNameOrLabel());
	}
	else
	{

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
		if (Actor->ActorHasTag(AcceptableActorTag))
		{
			return Actor;
		}
	}

	return nullptr;
}
