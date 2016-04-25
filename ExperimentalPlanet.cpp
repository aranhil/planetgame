// Fill out your copyright notice in the Description page of Project Settings.

#include "PlanetGame.h"
#include "ExperimentalPlanet.h"
#include "ProceduralMeshComponent.h"


void SmallCubeSide::GenerateTriangles()
{

}

void LargeCubeSide::GenerateSmallCubeSides(int subdivisions)
{
	for (int row = 0; row < subdivisions; row++)
	{
		smallCubeSides.Add(TArray<SmallCubeSide*>());

		for (int col = 0; col < subdivisions; col++)
		{
			smallCubeSides[row].Add(new SmallCubeSide());

			FVector smallPosition = position - topVec + topVec * ((float)row / subdivisions + 1.0f / 2.0f * subdivisions) 
											 - rightVec + rightVec * ((float)col / subdivisions + 1.0f / 2.0f * subdivisions);

			smallCubeSides[row][col]->position = smallPosition;
			smallCubeSides[row][col]->topVec = topVec * (1.0f / 2.0f * subdivisions);
			smallCubeSides[row][col]->rightVec = rightVec * (1.0f / 2.0f * subdivisions);
		}
	}

	this->subdivisions = subdivisions;
}

void LargeCubeSide::LinkSmallCubeSides()
{
	for (int row = 0; row < subdivisions; row++)
	{
		for (int col = 0; col < subdivisions; col++)
		{
			if (col != 0)
			{
				smallCubeSides[row][col]->left = smallCubeSides[row][col - 1];
			}
			else
			{
				smallCubeSides[row][col]->left = left->smallCubeSides[row][subdivisions - 1];
			}

			if (col != subdivisions - 1)
			{
				smallCubeSides[row][col]->right = smallCubeSides[row][col + 1];
			}
			else
			{
				smallCubeSides[row][col]->right = right->smallCubeSides[row][0];
			}

			if (row != 0)
			{
				smallCubeSides[row][col]->top = smallCubeSides[row - 1][col];
			}
			else
			{
				smallCubeSides[row][col]->top = top->smallCubeSides[subdivisions - 1][col];
			}

			if (row != subdivisions - 1)
			{
				smallCubeSides[row][col]->bottom = smallCubeSides[row + 1][col];
			}
			else
			{
				smallCubeSides[row][col]->bottom = bottom->smallCubeSides[0][col];
			}
		}
	}
}

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
	LargeCubeSide top;
	LargeCubeSide bottom;
	LargeCubeSide left;
	LargeCubeSide right;
	LargeCubeSide front;
	LargeCubeSide back;

	const int subdivisions = 2;

	top.GenerateSmallCubeSides(subdivisions);
	bottom.GenerateSmallCubeSides(subdivisions);
	left.GenerateSmallCubeSides(subdivisions);
	right.GenerateSmallCubeSides(subdivisions);
	front.GenerateSmallCubeSides(subdivisions);
	back.GenerateSmallCubeSides(subdivisions);

	// top
	top.top = &back;
	top.topSide = LargeCubeSideLinkage::eReversed;

	top.bottom = &front;
	top.bottomSide = LargeCubeSideLinkage::eDirect;

	top.left = &left;
	top.leftSide = LargeCubeSideLinkage::eDirect;

	top.right = &right;
	top.rightSide = LargeCubeSideLinkage::eReversed;

	// front
	front.top = &top;
	front.topSide = LargeCubeSideLinkage::eDirect;

	//TArray<FVector> Vertices;
	//Vertices.Add(FVector::UpVector * PlanetRadius);
	//Vertices.Add(FVector::UpVector * -PlanetRadius);
	//Vertices.Add(FVector::RightVector * PlanetRadius);
	//Vertices.Add(FVector::RightVector * -PlanetRadius);
	//Vertices.Add(FVector::ForwardVector * PlanetRadius);
	//Vertices.Add(FVector::ForwardVector * -PlanetRadius);

	//TArray<int32> Triangles;
	//Triangles.Add(0);
	//Triangles.Add(2);
	//Triangles.Add(4);

	//Triangles.Add(0);
	//Triangles.Add(4);
	//Triangles.Add(3);

	//Triangles.Add(0);
	//Triangles.Add(3);
	//Triangles.Add(5);

	//Triangles.Add(0);
	//Triangles.Add(5);
	//Triangles.Add(2);

	//Triangles.Add(1);
	//Triangles.Add(4);
	//Triangles.Add(2);

	//Triangles.Add(1);
	//Triangles.Add(3);
	//Triangles.Add(4);

	//Triangles.Add(1);
	//Triangles.Add(5);
	//Triangles.Add(3);

	//Triangles.Add(1);
	//Triangles.Add(2);
	//Triangles.Add(5);

	//TArray<FVector> normals;
	//normals.Add(FVector::UpVector);
	//normals.Add(FVector::UpVector);
	//normals.Add(FVector::RightVector);
	//normals.Add(FVector::RightVector);
	//normals.Add(FVector::ForwardVector);
	//normals.Add(FVector::ForwardVector);

	//TArray<FVector2D> UV0;
	//UV0.Add(FVector2D(0, 0));
	//UV0.Add(FVector2D(0, 10));
	//UV0.Add(FVector2D(10, 10));
	//UV0.Add(FVector2D(0, 0));
	//UV0.Add(FVector2D(0, 10));
	//UV0.Add(FVector2D(10, 10));

	//TArray<FColor> vertexColors;
	//vertexColors.Add(FColor(100, 100, 200, 255));
	//vertexColors.Add(FColor(100, 200, 100, 255));
	//vertexColors.Add(FColor(200, 100, 100, 255));
	//vertexColors.Add(FColor(200, 200, 100, 255));
	//vertexColors.Add(FColor(100, 200, 200, 255));
	//vertexColors.Add(FColor(200, 100, 200, 255));


	//TArray<FProcMeshTangent> tangents;
	//tangents.Add(FProcMeshTangent(1, 1, 1));
	//tangents.Add(FProcMeshTangent(1, 1, 1));
	//tangents.Add(FProcMeshTangent(1, 1, 1));
	//tangents.Add(FProcMeshTangent(1, 1, 1));
	//tangents.Add(FProcMeshTangent(1, 1, 1));
	//tangents.Add(FProcMeshTangent(1, 1, 1));


	//mesh->CreateMeshSection(1, vertices, Triangles, normals, UV0, vertexColors, tangents, false);

	// With default options
	//Mesh->CreateMeshSection(1, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), vertexColors, TArray<FProcMeshTangent>(), true);
}

