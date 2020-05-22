// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "demo3GameMode.h"
#include "demo3Character.h"
#include "UObject/ConstructorHelpers.h"

Ademo3GameMode::Ademo3GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
