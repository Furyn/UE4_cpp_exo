// Fill out your copyright notice in the Description page of Project Settings.


#include "TestJumpPad.h"
#include "GameFramework/Character.h"

// Sets default values
ATestJumpPad::ATestJumpPad()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("StaticMesh'/Game/Geometry/Meshes/1M_Cube.1M_Cube'"));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NomMesh"));
	if (CubeMesh.Succeeded())
	{
		StaticMesh->SetStaticMesh(CubeMesh.Object);
	}

	StaticMesh->SetSimulatePhysics(true);
	StaticMesh->OnComponentHit.AddDynamic(this, &ATestJumpPad::OnHit);

	RootComponent = StaticMesh;
}

// Called when the game starts or when spawned
void ATestJumpPad::BeginPlay()
{
	Super::BeginPlay();

	GLog->Log("Bonjour");
}

// Called every frame
void ATestJumpPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestJumpPad::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ACharacter* Character = Cast<ACharacter>(OtherActor);
	if (Character == nullptr) //< if (!Character)
		return;

	GLog->Log("Launch");
	Character->LaunchCharacter(FVector(0, 0, 1000), false, true);
}