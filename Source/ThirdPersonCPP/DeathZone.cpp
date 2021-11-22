#include "DeathZone.h"
#include "DrawDebugHelpers.h"

ADeathZone::ADeathZone()
{
	PrimaryActorTick.bCanEverTick = false;
	OnActorBeginOverlap.AddDynamic(this, &ADeathZone::OnOverlapBegin);

}

void ADeathZone::BeginPlay()
{
	Super::BeginPlay();
	DrawDebugBox(GetWorld(),GetActorLocation(),GetComponentsBoundingBox().GetExtent(), FColor::Green,true,-1,0,5);
}

void ADeathZone::OnOverlapBegin(class AActor* OverlappedComp, AActor* OtherActor)
{
	if (OtherActor && (OtherActor != this)) {
		Player = Cast<AThirdPersonCPPCharacter>(OtherActor);
		if(Player)
		{
			Player->Die();
		}
	}
}

void ADeathZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

