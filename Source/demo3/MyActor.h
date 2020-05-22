// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActor.generated.h"

UCLASS()
class DEMO3_API AMyActor : public AActor
{
	GENERATED_BODY()

public:
	struct Points {
		TArray<FVector> point;
	};

	Points points;

public:
	TArray<Points> refPoints;

public:
	TArray<FVector> difPoints;

public:
	// Sets default values for this actor's properties
	AMyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void RayLine();

public:
	void drawLine(FVector startPoint, FVector endPoint, int R, int G, int B);

public:
	void drawPoint(FVector point, int R, int G, int B);

public:
	void drawpowerpoint(float x, float y, float z, int R, int G, int B);

};
