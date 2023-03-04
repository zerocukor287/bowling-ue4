// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Pin.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/TriggerVolume.h"

// Sets default values
ACPP_Pin::ACPP_Pin() :
	isStanding (true),
	trigger(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	VisualMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder"));

	if (CubeVisualAsset.Succeeded())
	{
		VisualMesh->SetStaticMesh(CubeVisualAsset.Object);
		VisualMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		VisualMesh->SetRelativeScale3D({ .25f,.25f, 0.75f });

		VisualMesh->SetMobility(EComponentMobility::Movable);
		VisualMesh->SetSimulatePhysics(true);
		VisualMesh->SetMassOverrideInKg(NAME_None, 2.0f);

	}

	// Get the proper trigger for point calculation
	TArray<AActor*> triggers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATriggerVolume::StaticClass(), triggers);
	for (auto potentialTrigger : triggers) {
		if (potentialTrigger->GetFName().ToString() == "PinTrigger")
			trigger = potentialTrigger;
	}

}

void ACPP_Pin::setPosition(FVector position)
{
	VisualMesh->SetPhysicsLinearVelocity({ 0.0f, 0.0f, 0.0f });
	VisualMesh->SetPhysicsAngularVelocity({ 0.0f, 0.0f, 0.0f });
	FQuat rotation{ 0.0f, 90.0f, 0.0f, 1.0f };
	SetActorRotation(rotation);
	SetActorLocation(position);
}

// Called when the game starts or when spawned
void ACPP_Pin::BeginPlay()
{
	Super::BeginPlay();
	
	VisualMesh->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Pin::OverlapBegin);
	VisualMesh->OnComponentEndOverlap.AddDynamic(this, &ACPP_Pin::OverlapEnd);
}

// Called every frame
void ACPP_Pin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPP_Pin::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GEngine != nullptr && OtherActor == trigger) {
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Pin Up"));
		isStanding = true;
	}
}

void ACPP_Pin::OverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GEngine != nullptr && OtherActor == trigger) {
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("Pin Down"));
		isStanding = false;
	}
}

