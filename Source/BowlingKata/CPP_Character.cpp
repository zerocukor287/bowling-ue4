// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Character.h"
#include "CPP_BowlingBall.h"
#include "CPP_BowlingState.h"

// Sets default values
ACPP_Character::ACPP_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACPP_Character::BeginPlay()
{
	Super::BeginPlay();

	// regsiter myself to game state
	auto* gameState = GetWorld()->GetGameState<ACPP_BowlingState>();
	gameState->player = this;
	gameState->spawnPins();
}

// Called every frame
void ACPP_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACPP_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("LaunchBall"), IE_Pressed, this, &ACPP_Character::LaunchBall);
}

void ACPP_Character::LaunchBall()
{
	if (BallCount <= 0)
		return;

	BallCount--;
	auto location = GetActorLocation();
	auto forward = GetActorForwardVector();
	auto rotation = GetActorRotation();
	FTransform transform{ rotation, location + forward*64.0f };
	auto* SpawnedActor1 = GetWorld()->SpawnActorDeferred<ACPP_BowlingBall>(ACPP_BowlingBall::StaticClass(), transform);
	if (SpawnedActor1 != nullptr) {
		SpawnedActor1->Velocity = forward * 64.0f + GetVelocity();
		SpawnedActor1->player = this;
		SpawnedActor1->FinishSpawning(transform);
	}
}
