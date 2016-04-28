// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "ExperimentalPlanet.generated.h"

struct SmallCubeSide
{
	SmallCubeSide* top;
	SmallCubeSide* bottom;
	SmallCubeSide* left;
	SmallCubeSide* right;

	FVector position;
	FVector rightVec;
	FVector topVec;

	void GenerateTriangles(TArray<int32>& triangles, TArray<FVector>& vertices);

	bool trianglesGenerated;
	TArray<int32> triangles;
	TArray<FVector> vertices;
};

enum LargeCubeSideLinkage
{
	eDirect,
	eReversed
};

struct LargeCubeSide
{
	LargeCubeSide* top;
	LargeCubeSide* bottom;
	LargeCubeSide* left;
	LargeCubeSide* right;

	LargeCubeSideLinkage topSide;
	LargeCubeSideLinkage bottomSide;
	LargeCubeSideLinkage leftSide;
	LargeCubeSideLinkage rightSide;

	int subdivisions;

	FVector position;
	FVector rightVec;
	FVector topVec;

	TArray<TArray<SmallCubeSide*>> smallCubeSides;

	void GenerateSmallCubeSides(int subdivisions);
	void GenerateTriangles(TArray<int32>& triangles, TArray<FVector>& vertices);
	void LinkSmallCubeSides();
};

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

	UPROPERTY(EditAnywhere)
	int Subdivisions;

	int oldSubdivisions;
	int oldPlanetRadius;
	
	void PlanetTopologyChanged();
};
