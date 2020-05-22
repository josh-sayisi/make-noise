// Fill out your copyright notice in the Description page of Project Settings.

#include "MyActor.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "tinyxml.h"
#include "tinystr.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include <fstream>
using namespace std;

// Sets default values
AMyActor::AMyActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();

	RayLine();

}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyActor::RayLine()
{
	FString _FilePath = FPaths::ProjectDir() + "twice ref.xml";

	char* path = TCHAR_TO_UTF8(*_FilePath);

	TiXmlDocument doc(path);

	if (!doc.LoadFile())	//judge whether the file exist 
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White, TEXT("file open failure!"));
		UE_LOG(LogTemp, Log, TEXT("file open failure!"));
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White, TEXT("work"));
		UE_LOG(LogTemp, Log, TEXT("work!"));
	}

	TiXmlHandle hDoc(&doc);
	TiXmlElement* pElem;
	TiXmlElement* pNode;
	TiXmlElement* pChildNode;
	TiXmlHandle hRoot(0);

	pElem = hDoc.FirstChildElement().Element();// should always have a valid root but handle gracefully if it does
	if (!pElem)
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White, TEXT("no root node"));
	UE_LOG(LogTemp, Log, TEXT("no root node"));

	// save this for later
	hRoot = TiXmlHandle(pElem);

	pElem = hRoot.FirstChild().Element();
	const char *pValue;
	const TCHAR* msg;
	FVector start, end, f;

	while (true)
	{
		const char *pText;
		pNode = TiXmlHandle(pElem).FirstChild().Element();
		pValue = pElem->Value();
		FString str = pValue;
		if (str == ("StartPoint"))
		{
			pText = pNode->GetText();
			msg = UTF8_TO_TCHAR(pText);
			start.X = FCString::Atof(msg);
			pNode = pNode->NextSiblingElement();
			pText = pNode->GetText();
			msg = UTF8_TO_TCHAR(pText);
			start.Y = FCString::Atof(msg);
			pNode = pNode->NextSiblingElement();
			pText = pNode->GetText();
			msg = UTF8_TO_TCHAR(pText);
			start.Z = FCString::Atof(msg);

			pElem = pElem->NextSiblingElement();
		}
		else if (str == ("EndPoint"))
		{
			pText = pNode->GetText();
			msg = UTF8_TO_TCHAR(pText);
			end.X = FCString::Atof(msg);
			pNode = pNode->NextSiblingElement();
			pText = pNode->GetText();
			msg = UTF8_TO_TCHAR(pText);
			end.Y = FCString::Atof(msg);
			pNode = pNode->NextSiblingElement();
			pText = pNode->GetText();
			msg = UTF8_TO_TCHAR(pText);
			end.Z = FCString::Atof(msg);

			break;
		}

		else if (str == ("RefPoint"))
		{
			while (true)
			{
				if (pNode == NULL)
					break;
				pChildNode = TiXmlHandle(pNode).FirstChild().Element();
				pText = pChildNode->GetText();
				msg = UTF8_TO_TCHAR(pText);
				f.X = FCString::Atof(msg);
				pChildNode = pChildNode->NextSiblingElement();
				pText = pChildNode->GetText();
				msg = UTF8_TO_TCHAR(pText);
				f.Y = FCString::Atof(msg);
				pChildNode = pChildNode->NextSiblingElement();
				pText = pChildNode->GetText();
				msg = UTF8_TO_TCHAR(pText);
				f.Z = FCString::Atof(msg);
				points.point.Emplace(f);

				pNode = pNode->NextSiblingElement();
			}
			refPoints.Emplace(points);
			points.point.Empty();
			pElem = pElem->NextSiblingElement();

		}
		else
		{
			pText = pNode->GetText();
			msg = UTF8_TO_TCHAR(pText);
			f.X = FCString::Atof(msg);
			pNode = pNode->NextSiblingElement();
			pText = pNode->GetText();
			msg = UTF8_TO_TCHAR(pText);
			f.Y = FCString::Atof(msg);
			pNode = pNode->NextSiblingElement();
			pText = pNode->GetText();
			msg = UTF8_TO_TCHAR(pText);
			f.Z = FCString::Atof(msg);
			difPoints.Emplace(f);

			pElem = pElem->NextSiblingElement();
		}
	}

	//start point
	drawPoint(start, 0, 255, 0);

	//end point
	drawPoint(end, 0, 0, 255);

	//perpendicular incidence
	drawLine(start, end,0,0,0);

	//reflect
	for (int j = 0; j < refPoints.Num(); j++)
	{
		int num = refPoints[j].point.Num();
		drawLine(start, refPoints[j].point[0], (j+1) * 15 % 256, (j+1) * 25 % 256, (j+1) * 35 % 256);
		drawLine(refPoints[j].point[num - 1], end, (num+1) * (j+1) * 15 % 256, (num+1) * (j+1) * 25 % 256, (num+1) * (j+1) * 35 % 256);
		if (num == 1)
			continue;

		else
		{
			for (int i = 1; i < num; i++)
				drawLine(refPoints[j].point[i - 1], refPoints[j].point[i], (i+1) * (j+1) * 15 % 256, (i+1) * (j+1) * 25 % 256, (i+1) * (j+1) * 35 % 256);
		}
		//GEngine->AddOnScreenDebugMessage(-1, 1000, FColor::White, FString::SanitizeFloat(lines[i].endPoint.X));

	}

	//diffraction
	for (int i = 0; i < difPoints.Num(); i++)
	{
		drawLine(start, difPoints[i], (i+1) * 15 % 256, (i+1) * 25 % 256, (i+1) * 35 % 256);
		drawLine(difPoints[i], end, (i+1) * 25 % 256, (i+1) * 35 % 256, (i+1) * 45 % 256);
	}

	//drawpowerpoint(-900, 900, 920, 0, 255, 0);
}

void AMyActor::drawLine(FVector startPoint, FVector endPoint, int R, int G, int B)
{
	DrawDebugLine(
		GetWorld(),
		FVector(startPoint.X * 100 - 1900, -(startPoint.Y * 100 - 1500), startPoint.Z * 100 + 141),
		FVector(endPoint.X * 100 - 1900, -(endPoint.Y * 100 - 1500), endPoint.Z * 100 + 141),
		FColor(R,G,B),
		false,
		1000,
		0.0f,
		3.0f);
}

void AMyActor::drawPoint(FVector point, int R, int G, int B)
{
	DrawDebugPoint(
		GetWorld(),
		FVector(point.X * 100 - 1900, -(point.Y * 100 - 1500), point.Z * 100 + 141),
		20.f,
		FColor(R,G,B),
		false,
		1000,
		0.0f
	);
}

void AMyActor::drawpowerpoint(float x, float y, float z, int R, int G, int B)
{
	DrawDebugPoint(
		GetWorld(),
		FVector(x, y, z),
		50.f,
		FColor(R, G, B),
		false,
		1000,
		0.0f
	);
}
