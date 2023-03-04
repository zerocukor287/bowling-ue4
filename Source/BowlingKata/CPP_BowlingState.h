// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CPP_Pin.h"
#include "CPP_BowlingState.generated.h"

/**
 * 
 */
class ACPP_Character;

UCLASS()
class BOWLINGKATA_API ACPP_BowlingState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void ballFallOffMap();

	UFUNCTION()
	void NextBall();

	UFUNCTION()
	void NextFrame();

	UFUNCTION()
	void GiveTheBallBack();

	UFUNCTION()
	void spawnPins();

	UFUNCTION()
	void destroyPins();

	UFUNCTION()
	void resetRemainigPins();

	UFUNCTION()
	void calculateScore(bool midFrame);


	UPROPERTY(EditAnywhere)
	ACPP_Character* player;

private:
	TMap<ACPP_Pin*, FVector> pins;

	int32 roll{0};
	int32 frame{0};
	int32 totalScore{0};
	int32 remainingPins{ 0 };
	bool previousStrike{ false };
	bool previousSpare{ false };

};
