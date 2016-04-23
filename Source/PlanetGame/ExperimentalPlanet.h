// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "ExperimentalPlanet.generated.h"

UCLASS()
class PLANETGAME_API AExperimentalPlanet : public AActor
{
	GENERATED_BODY()
	
	UProceduralMeshComponent* Mesh;
public:	
	// Sets default values for this actor's properties
	AExperimentalPlanet();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere)
	int PlanetRadius;
	
	void PlanetRadiusChanged();
};
