// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_BowlingState.h"
#include "CPP_Character.h"

#include "Kismet/GameplayStatics.h"


void ACPP_BowlingState::ballFallOffMap()
{
	if (roll == 0) {
		// first ball in a frame
		calculateScore(true);
		if (remainingPins == 0) {
			previousStrike = true;
			if (frame < 9) {
				if (GEngine != nullptr)
					GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("Strike!"));
			
				NextFrame();
			}
			else {
				// Last frame, reset the pins and give one more ball
				if (GEngine != nullptr)
					GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("Strike! In the last frame."));

				destroyPins();
				NextBall();
			}
		}
		else {
			if (GEngine != nullptr)
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("Proceed to the next roll."));

			NextBall();
		}
	}
	else if (roll == 1) {
		// second ball in a frame
		bool strikeInTheLastFrame = previousStrike;
		calculateScore(false);
		if (remainingPins == 0) {
			if (frame < 9) {
				previousSpare = true;
				if (GEngine != nullptr)
					GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("Spare!"));

				NextFrame();
			}
			else {
				// last frame, give one more ball
				if (strikeInTheLastFrame)
					previousStrike = true;
				else
					previousSpare = true;

				if (GEngine != nullptr)
					GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("Spare! In the last frame"));

				NextBall();
			}
		}
		else {
			if (GEngine != nullptr)
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("2nd roll is over, next frame"));

			NextFrame();
		}
	}
	else {
		// special case, 3rd roll
		calculateScore(false);
	}

	if (frame >= 9 && !(previousSpare || previousStrike)) {
		if (GEngine != nullptr)
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, FString::Printf(TEXT("Game over! You score: %d"), totalScore));
		
		player->BallCount = 0;
	}
}

void ACPP_BowlingState::NextBall()
{
	roll++;
	resetRemainigPins();
	GiveTheBallBack();
}

void ACPP_BowlingState::NextFrame()
{
	frame++;
	roll = 0;
	destroyPins();
	GiveTheBallBack();

	if (GEngine != nullptr)
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("Score: %d"), totalScore));
}

void ACPP_BowlingState::GiveTheBallBack()
{
	// give the ball back to the player
	//if (GEngine != nullptr)
	//	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("I'm going to give the player one more ball"));

	if (player != nullptr)
		player->BallCount = player->BallCount + 1;
}

void ACPP_BowlingState::spawnPins()
{
	const FVector location{-1420.0f, -5.0f, 100.0f};

	for (int row = 0; row < 4; ++row) {
		FVector rowLocation = location - FVector{ 50.0f * row, 0.0f, 0.0f };
		for (int column = 0; column <= row; ++column) {
			FVector finalLocation = rowLocation - FVector{ 0.0f, 20.0f * row - 40.0f * column, 0.0f };

			pins.Add(GetWorld()->SpawnActor<ACPP_Pin>(finalLocation, FRotator()), finalLocation);
		}
	}
}

void ACPP_BowlingState::destroyPins()
{
	for (auto pin : pins) {
		pin.Key->setPosition(pin.Value);
	}
}

void ACPP_BowlingState::resetRemainigPins()
{
	FVector outOfMap{600.0f, -100.0f, -200.0f};
	int32 offset = 0;
	for (auto& pin : pins) {
		if (pin.Key->isStanding) {
			pin.Key->setPosition(pin.Value);
		} else {
			pin.Key->setPosition(outOfMap + FVector{ 0.0f, 20.0f * offset, 0.0f });
			offset++;
		}
	}
}

void ACPP_BowlingState::calculateScore(bool midFrame)
{
	int32 pinsDown = 0;
	for (const auto& pin : pins) {
		if (!pin.Key->isStanding)
			pinsDown++;
	}
	remainingPins = 10 - pinsDown;


	if (previousStrike && (!midFrame || pinsDown == 10)) {
		// add again the pins at the end or when it's a strike again
		totalScore += pinsDown;
		previousStrike = false;
	}

	if (previousSpare && midFrame) {
		// only add the first roll
		totalScore += pinsDown;
		previousSpare = false;
	}

	if (!midFrame || pinsDown == 10)
		totalScore += pinsDown;
}
