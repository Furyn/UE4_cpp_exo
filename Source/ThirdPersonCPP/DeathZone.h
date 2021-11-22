#pragma once

#include "CoreMinimal.h"
#include "ThirdPersonCPPCharacter.h"
#include "Engine/TriggerBox.h"
#include "DeathZone.generated.h"

UCLASS()
class THIRDPERSONCPP_API ADeathZone : public ATriggerBox
{
	GENERATED_BODY()
	
public:	
	ADeathZone();
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;
	

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
    void OnOverlapBegin(class AActor* OverlappedComp, class AActor* OtherActor);
	AThirdPersonCPPCharacter* Player;

};
