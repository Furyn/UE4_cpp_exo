// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThirdPersonCPPCharacter.h"
#include "GameFramework/Actor.h"
#include "TriggerHealth.generated.h"

UCLASS()
class THIRDPERSONCPP_API ATriggerHealth : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATriggerHealth();
	UPROPERTY(EditAnywhere)
	int Dammage = 0;

	UPROPERTY(EditAnywhere)
	float TimerWait = 2.0f;
    	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMesh;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void HealtChange();

	UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedComp, class AActor* OtherActor);
	
	UFUNCTION()
	void OnOverlapEnd(class AActor* OverlappedComp, class AActor* OtherActor);

	AThirdPersonCPPCharacter* Player;
	FTimerHandle TimerHandle;
	
};
