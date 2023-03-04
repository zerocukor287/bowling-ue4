// Copyright Epic Games, Inc. All Rights Reserved.


#include "BowlingKataGameModeBase.h"

#include "CPP_BowlingState.h"

ABowlingKataGameModeBase::ABowlingKataGameModeBase()
{
    static ConstructorHelpers::FObjectFinder<UBlueprint> Blueprint(TEXT("Blueprint'/Game/ThirdPersonBP/Blueprints/ThirdPersonCharacter'"));
    if (Blueprint.Object) {
        DefaultPawnClass = (UClass*)Blueprint.Object->GeneratedClass;
    }

    GameStateClass = ACPP_BowlingState::StaticClass();
}
