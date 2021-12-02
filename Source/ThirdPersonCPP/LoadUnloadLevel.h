#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LoadUnloadLevel.generated.h"

UCLASS()
class THIRDPERSONCPP_API ALoadUnloadLevel : public AActor
{
	GENERATED_BODY()
	
public:	
	ALoadUnloadLevel();
	UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* StaticMesh;
    	
	UPROPERTY(EditAnywhere)
	TArray<FName> LayersToLoadOnBegin;
	UPROPERTY(EditAnywhere)
    TArray<FName> LayersToLoadOnEnd;
    
	UPROPERTY(EditAnywhere)
	TArray<FName> LayersToUnLoadOnBegin;
	UPROPERTY(EditAnywhere)
	TArray<FName> LayersToUnLoadOnEnd;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);
	
	UFUNCTION()
	void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);

	void Load(TArray<FName> LayersToLoad) const;
	void Unload(TArray<FName> LayersToLoad) const;
};
