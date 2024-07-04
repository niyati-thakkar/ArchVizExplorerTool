// Fill out your copyright notice in the Description page of Project Settings.
#include "ConstructionActors/ArchVizDoorActor.h"
#include "Game/ArchVizPlayerController.h"


// Sets default values
AArchVizDoorActor::AArchVizDoorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create and set up the door frame component
	DoorFrameComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrameComponent"));
	RootComponent = DoorFrameComponent;

	DoorFrameComponent->SetRelativeLocation(FVector(200.0f, 0.0f, 0.0f));
	DoorFrameComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	DoorFrameComponent->SetRelativeScale3D(FVector(1, 1.1f, 1));

	// Create and set up the door component
	DoorComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorComponent"));
	DoorComponent->SetupAttachment(DoorFrameComponent);
	DoorComponent->SetRelativeLocation(FVector(0.0f, 45.0f, 0.0f));

	// Load the door frame mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DoorFrameMeshAsset(TEXT("/Game/StarterContent/Props/SM_DoorFrame.SM_DoorFrame"));
	if (DoorFrameMeshAsset.Succeeded())
	{
		DoorFrameMesh = DoorFrameMeshAsset.Object;
		DoorFrameComponent->SetStaticMesh(DoorFrameMesh);
	}

	// Load the door mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DoorMeshAsset(TEXT("/Game/StarterContent/Props/SM_Door.SM_Door"));
	if (DoorMeshAsset.Succeeded())
	{
		DoorMesh = DoorMeshAsset.Object;
		DoorComponent->SetStaticMesh(DoorMesh);
	}

}

// Called when the game starts or when spawned
void AArchVizDoorActor::BeginPlay()
{
	Super::BeginPlay();
	
}
AArchVizWallActor* AArchVizDoorActor::GetWallActor()
{
	return WallActor;
}

void AArchVizDoorActor::RotateActor(FRotator ApplyRotation)
{
	Super::RotateActor(ApplyRotation);
	FRotator currentRotation = DoorComponent->GetRelativeRotation();

	double newYaw = fmod(currentRotation.Yaw + 90.0, 180.0);
	if (newYaw < 0)
	{
		newYaw += 360.0;
	}

	// Create a new rotation with the updated Yaw
	FRotator newRotation = FRotator(currentRotation.Pitch, newYaw, currentRotation.Roll);

	// Set the actor's rotation to the new rotation
	DoorComponent->SetRelativeRotation(newRotation);

}

// Called every frame
void AArchVizDoorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetIsMoving() && GetPlayerController()) {
		FHitResult HitResult = GetPlayerController()->GetMouseLocation({ this });
		SetActorLocation(HitResult.Location);
		if(AArchVizWallActor* Actor = Cast<AArchVizWallActor>(HitResult.GetActor()))
		{
			WallActor = Actor;
			DoorFrameComponent->SetWorldRotation(WallActor->GetRotation() + FRotator(0,90,0));
		}
		
	}
}

