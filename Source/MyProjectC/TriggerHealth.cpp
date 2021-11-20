// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerHealth.h"

// Sets default values
ATriggerHealth::ATriggerHealth()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NomMesh"));

	StaticMesh->OnComponentHit.AddDynamic(this, &AJumpPad::OnHit);

	RootComponent = StaticMesh;

}


// Called when the game starts or when spawned
void ATriggerHealth::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATriggerHealth::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

