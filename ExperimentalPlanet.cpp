// Fill out your copyright notice in the Description page of Project Settings.

#include "PlanetGame.h"
#include "ExperimentalPlanet.h"
#include "ProceduralMeshComponent.h"


void SmallCubeSide::GenerateTriangles(TArray<int32>& triangles, TArray<FVector>& vertices)
{
	triangles.Add(vertices.Add(position + this->topVec - this->rightVec));
	triangles.Add(vertices.Add(position + this->topVec + this->rightVec));
	triangles.Add(vertices.Add(position - this->topVec - this->rightVec));

	triangles.Add(vertices.Add(position - this->topVec - this->rightVec));
	triangles.Add(vertices.Add(position + this->topVec + this->rightVec));
	triangles.Add(vertices.Add(position - this->topVec + this->rightVec));
}

void LargeCubeSide::GenerateSmallCubeSides(int subdivisions)
{
	for (int row = 0; row < subdivisions; row++)
	{
		smallCubeSides.Add(TArray<SmallCubeSide*>());

		for (int col = 0; col < subdivisions; col++)
		{
			smallCubeSides[row].Add(new SmallCubeSide());

			FVector smallPosition = position - topVec + topVec * 2 * ((float)row / subdivisions) + (topVec / subdivisions)
											 - rightVec + rightVec * 2 * ((float)col / subdivisions) + (rightVec / subdivisions);

			smallCubeSides[row][col]->position = smallPosition;
			smallCubeSides[row][col]->topVec = topVec * (1.0f / subdivisions);
			smallCubeSides[row][col]->rightVec = rightVec * (1.0f / subdivisions);
		}
	}

	this->subdivisions = subdivisions;
}

void PreprocessVertex(FVector& v)
{
	float x2 = v.X * v.X;
	float y2 = v.Y * v.Y;
	float z2 = v.Z * v.Z;
	FVector s;
	s.X = v.X * FMath::Sqrt(1.0f - y2 / 2.0f - z2 / 2.0f + y2 * z2 / 3.0f);
	s.Y = v.Y * FMath::Sqrt(1.0f - x2 / 2.0f - z2 / 2.0f + x2 * z2 / 3.0f);
	s.Z = v.Z * FMath::Sqrt(1.0f - x2 / 2.0f - y2 / 2.0f + x2 * y2 / 3.0f);
	v = s;
}

void LargeCubeSide::GenerateTriangles(TArray<int32>& triangles, TArray<FVector>& vertices)
{
	for (int row = 0; row < subdivisions; row++)
	{
		for (int col = 0; col < subdivisions; col++)
		{
			smallCubeSides[row][col]->GenerateTriangles(triangles, vertices);

			for (int i = 1; i < 7; i++)
			{
				vertices[vertices.Num() - i] /= this->position.Size();
				PreprocessVertex(vertices[vertices.Num() - i]);
				vertices[vertices.Num() - i] *= this->position.Size();
			}
		}
	}
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

	oldPlanetRadius = PlanetRadius = 10000;
	oldSubdivisions = Subdivisions = 200;
}

// Called when the game starts or when spawned
void AExperimentalPlanet::BeginPlay()
{
	Super::BeginPlay();
	PlanetTopologyChanged();
}

// Called every frame
void AExperimentalPlanet::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (oldPlanetRadius != PlanetRadius || oldSubdivisions != Subdivisions)
	{
		PlanetTopologyChanged();
		oldPlanetRadius = PlanetRadius;
		oldSubdivisions = Subdivisions;
	}
}

void AExperimentalPlanet::PlanetTopologyChanged()
{
	LargeCubeSide top;
	LargeCubeSide bottom;
	LargeCubeSide left;
	LargeCubeSide right;
	LargeCubeSide front;
	LargeCubeSide back;

	top.position = FVector::UpVector * PlanetRadius;
	top.rightVec = FVector::RightVector * PlanetRadius;
	top.topVec = -FVector::ForwardVector * PlanetRadius;

	front.position = FVector::ForwardVector * PlanetRadius;
	front.rightVec = FVector::RightVector * PlanetRadius;
	front.topVec = FVector::UpVector * PlanetRadius;

	left.position = -FVector::RightVector * PlanetRadius;
	left.rightVec = FVector::ForwardVector * PlanetRadius;
	left.topVec = FVector::UpVector * PlanetRadius;

	bottom.position = -FVector::UpVector * PlanetRadius;
	bottom.rightVec = FVector::RightVector * PlanetRadius;
	bottom.topVec = FVector::ForwardVector * PlanetRadius;

	right.position = FVector::RightVector * PlanetRadius;
	right.rightVec = -FVector::ForwardVector * PlanetRadius;
	right.topVec = FVector::UpVector * PlanetRadius;

	back.position = -FVector::ForwardVector * PlanetRadius;
	back.rightVec = -FVector::RightVector * PlanetRadius;
	back.topVec = FVector::UpVector * PlanetRadius;

	top.GenerateSmallCubeSides(Subdivisions);
	bottom.GenerateSmallCubeSides(Subdivisions);
	left.GenerateSmallCubeSides(Subdivisions);
	right.GenerateSmallCubeSides(Subdivisions);
	front.GenerateSmallCubeSides(Subdivisions);
	back.GenerateSmallCubeSides(Subdivisions);

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

	front.bottom = &bottom;
	front.bottomSide = LargeCubeSideLinkage::eDirect;

	front.left = &left;
	front.leftSide = LargeCubeSideLinkage::eDirect;

	front.right = &right;
	front.rightSide = LargeCubeSideLinkage::eDirect;

	// left
	left.top = &top;
	left.topSide = LargeCubeSideLinkage::eReversed;

	left.bottom = &bottom;
	left.bottomSide = LargeCubeSideLinkage::eDirect;

	left.left = &back;
	left.leftSide = LargeCubeSideLinkage::eDirect;

	left.right = &front;
	left.rightSide = LargeCubeSideLinkage::eDirect;

	// bottom
	bottom.top = &front;
	bottom.topSide = LargeCubeSideLinkage::eDirect;

	bottom.bottom = &back;
	bottom.bottomSide = LargeCubeSideLinkage::eReversed;

	bottom.left = &left;
	bottom.leftSide = LargeCubeSideLinkage::eDirect;

	bottom.right = &right;
	bottom.rightSide = LargeCubeSideLinkage::eReversed;

	// right
	right.top = &top;
	right.topSide = LargeCubeSideLinkage::eDirect;

	right.bottom = &bottom;
	right.bottomSide = LargeCubeSideLinkage::eReversed;

	right.left = &front;
	right.leftSide = LargeCubeSideLinkage::eDirect;

	right.right = &back;
	right.rightSide = LargeCubeSideLinkage::eDirect;

	// back
	back.top = &top;
	back.topSide = LargeCubeSideLinkage::eReversed;

	back.bottom = &bottom;
	back.bottomSide = LargeCubeSideLinkage::eReversed;

	back.left = &right;
	back.leftSide = LargeCubeSideLinkage::eDirect;

	back.right = &left;
	back.rightSide = LargeCubeSideLinkage::eDirect;

	TArray<FVector> Vertices;
	TArray<int32> Triangles;

	top.GenerateTriangles(Triangles, Vertices);
	front.GenerateTriangles(Triangles, Vertices);
	left.GenerateTriangles(Triangles, Vertices);
	bottom.GenerateTriangles(Triangles, Vertices);
	right.GenerateTriangles(Triangles, Vertices);
	back.GenerateTriangles(Triangles, Vertices);

	//Vertices.Add(FVector::UpVector * PlanetRadius);
	//Vertices.Add(FVector::UpVector * -PlanetRadius);
	//Vertices.Add(FVector::RightVector * PlanetRadius);
	//Vertices.Add(FVector::RightVector * -PlanetRadius);
	//Vertices.Add(FVector::ForwardVector * PlanetRadius);
	//Vertices.Add(FVector::ForwardVector * -PlanetRadius);

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
	Mesh->CreateMeshSection(1, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), true);
}

