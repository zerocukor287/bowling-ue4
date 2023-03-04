// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_BowlingBall.h"
#include "CPP_Character.h"
#include "CPP_BowlingState.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/TriggerVolume.h"

// Sets default values
ACPP_BowlingBall::ACPP_BowlingBall() :
	player(nullptr),
	trigger(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	VisualMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));

	if (CubeVisualAsset.Succeeded())
	{
		VisualMesh->SetStaticMesh(CubeVisualAsset.Object);
		VisualMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		VisualMesh->SetRelativeScale3D({ .25,.25,.25 });

		VisualMesh->SetMobility(EComponentMobility::Movable);
		VisualMesh->SetSimulatePhysics(true);
		VisualMesh->SetMassOverrideInKg(NAME_None, 32.0f);

		VisualMesh->OnComponentBeginOverlap.AddDynamic(this, &ACPP_BowlingBall::OverlapBegin);
	}

	// Get the proper trigger for respawn
	TArray<AActor*> triggers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATriggerVolume::StaticClass(), triggers);
	for (auto potentialTrigger : triggers) {
		//if (GEngine != nullptr)
		//	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Class name %s"), *potentialTrigger->GetFName().ToString()));

		if (potentialTrigger->GetFName().ToString() == "BallResetTrigger")
			trigger = potentialTrigger;

	}
}

// Called when the game starts or when spawned
void ACPP_BowlingBall::BeginPlay()
{
	Super::BeginPlay();
	VisualMesh->SetPhysicsLinearVelocity(Velocity);
}

// Called every frame
void ACPP_BowlingBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPP_BowlingBall::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == trigger) {
		auto* gameState = GetWorld()->GetGameState<ACPP_BowlingState>();
		gameState->ballFallOffMap();
	}
}

