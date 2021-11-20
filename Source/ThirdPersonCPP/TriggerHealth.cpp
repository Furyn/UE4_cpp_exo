// Fill out your copyright notice in the Description page of Project Settings.

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)
#define printFString(text, fstring) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT(text), fstring))

#include "TriggerHealth.h"

#include <functional>

// Sets default values
ATriggerHealth::ATriggerHealth()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NomMesh"));

	StaticMesh->SetCollisionProfileName(TEXT("Trigger"));
	
	OnActorBeginOverlap.AddDynamic(this, &ATriggerHealth::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ATriggerHealth::OnOverlapEnd);

}    
 
// Called when the game starts or when spawned
void ATriggerHealth::BeginPlay()
{
	Super::BeginPlay();
	
}


void ATriggerHealth::OnOverlapBegin(class AActor* OverlappedComp, AActor* OtherActor)
{
	if (OtherActor && (OtherActor != this)) {
		Player = Cast<AThirdPersonCPPCharacter>(OtherActor);
		if(Player)
		{
			GetWorldTimerManager().SetTimer(TimerHandle,this,&ATriggerHealth::HealtChange,TimerWait,true,0);
		}
    }
}

void ATriggerHealth::OnOverlapEnd(AActor* OverlappedComp, AActor* OtherActor)
{
	if (OtherActor && (OtherActor != this))
	{
		Player = Cast<AThirdPersonCPPCharacter>(OtherActor);
		if(Player)
		{
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		}
	}
}



// Called every frame
void ATriggerHealth::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATriggerHealth::HealtChange()
{
	if(Player)
	{
		Player->ApplyHealthPoint(Dammage);
		printFString("Player life = %f", Player->Life);
	}
}

