#include "LoadUnloadLevel.h"
#include "ThirdPersonCPPCharacter.h"
#include "Kismet/GameplayStatics.h"

ALoadUnloadLevel::ALoadUnloadLevel()
{
    PrimaryActorTick.bCanEverTick = true;

    OnActorBeginOverlap.AddDynamic(this, &ALoadUnloadLevel::OnOverlapBegin);
    OnActorEndOverlap.AddDynamic(this, &ALoadUnloadLevel::OnOverlapEnd);
}

// Called when the game starts or when spawned
void ALoadUnloadLevel::BeginPlay()
{
    Super::BeginPlay();
}
void ALoadUnloadLevel::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
    AThirdPersonCPPCharacter* player = Cast<AThirdPersonCPPCharacter>(OtherActor);
    if (player)
    {
        Load(LayersToLoadOnBegin);
        Unload(LayersToUnLoadOnBegin);
    }
}

void ALoadUnloadLevel::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
    AThirdPersonCPPCharacter* player = Cast<AThirdPersonCPPCharacter>(OtherActor);
    if (player)
    {
        Load(LayersToLoadOnEnd);
        Unload(LayersToUnLoadOnEnd);
    }
}

void ALoadUnloadLevel::Tick(float DeltaTime){}

void ALoadUnloadLevel::Load(TArray<FName> LayersToLoad) const
{
    for (auto const LayerToLoad : LayersToLoad)
    {
        UGameplayStatics::LoadStreamLevel(this, LayerToLoad, true, false, FLatentActionInfo());
    }
}

void ALoadUnloadLevel::Unload(TArray<FName> LayersToUnLoad) const
{
    for (auto const LayerToLoad : LayersToUnLoad)
    {
        UGameplayStatics::UnloadStreamLevel(this, LayerToLoad, FLatentActionInfo(), false);
    }
}
