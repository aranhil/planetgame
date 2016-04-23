// Fill out your copyright notice in the Description page of Project Settings.

#include "PlanetGame.h"
#include "ExperimentalPlanet.h"
#include "ProceduralMeshComponent.h"

// Sets default values
AExperimentalPlanet::AExperimentalPlanet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Procedural Mesh"));
	Mesh->AttachTo(RootComponent);

	PlanetRadius = 100;
}

// Called when the game starts or when spawned
void AExperimentalPlanet::BeginPlay()
{
	Super::BeginPlay();
	PlanetRadiusChanged();
}

// Called every frame
void AExperimentalPlanet::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	PlanetRadiusChanged();
}

void AExperimentalPlanet::PlanetRadiusChanged()
{
	TArray<FVector> Vertices;
	Vertices.Add(FVector::UpVector * PlanetRadius);
	Vertices.Add(FVector::UpVector * -PlanetRadius);
	Vertices.Add(FVector::RightVector * PlanetRadius);
	Vertices.Add(FVector::RightVector * -PlanetRadius);
	Vertices.Add(FVector::ForwardVector * PlanetRadius);
	Vertices.Add(FVector::ForwardVector * -PlanetRadius);

	TArray<int32> Triangles;
	Triangles.Add(0);
	Triangles.Add(2);
	Triangles.Add(4);

	Triangles.Add(0);
	Triangles.Add(4);
	Triangles.Add(3);

	Triangles.Add(0);
	Triangles.Add(3);
	Triangles.Add(5);

	Triangles.Add(0);
	Triangles.Add(5);
	Triangles.Add(2);

	Triangles.Add(1);
	Triangles.Add(4);
	Triangles.Add(2);

	Triangles.Add(1);
	Triangles.Add(3);
	Triangles.Add(4);

	Triangles.Add(1);
	Triangles.Add(5);
	Triangles.Add(3);

	Triangles.Add(1);
	Triangles.Add(2);
	Triangles.Add(5);

	//TArray<FVector> normals;
	//normals.Add(FVector(1, 0, 0));
	//normals.Add(FVector(1, 0, 0));
	//normals.Add(FVector(1, 0, 0));

	//TArray<FVector2D> UV0;
	//UV0.Add(FVector2D(0, 0));
	//UV0.Add(FVector2D(0, 10));
	//UV0.Add(FVector2D(10, 10));

	//TArray<FColor> vertexColors;
	//vertexColors.Add(FColor(100, 100, 100, 100));
	//vertexColors.Add(FColor(100, 100, 100, 100));
	//vertexColors.Add(FColor(100, 100, 100, 100));


	//TArray<FProcMeshTangent> tangents;
	//tangents.Add(FProcMeshTangent(1, 1, 1));
	//tangents.Add(FProcMeshTangent(1, 1, 1));
	//tangents.Add(FProcMeshTangent(1, 1, 1));


	//mesh->CreateMeshSection(1, vertices, Triangles, normals, UV0, vertexColors, tangents, false);

	// With default options
	Mesh->CreateMeshSection(1, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), false);
}

