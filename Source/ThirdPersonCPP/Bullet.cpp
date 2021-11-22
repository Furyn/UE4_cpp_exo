#include "Bullet.h"

ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = false;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
    
    BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NomMesh"));
    if (SphereMesh.Succeeded()) 
    {
    	BulletMesh->SetStaticMesh(SphereMesh.Object);
    }
	
    BulletMesh->OnComponentHit.AddDynamic(this, &ABullet::OnHit);

    RootComponent = BulletMesh;
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();
}

void ABullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	FVector const* Location =  new FVector(Hit.Location.X,Hit.Location.Y,Hit.Location.Z);
	FRotator const* Rotation =  new FRotator(Hit.Normal.Rotation());
	GetWorld()->SpawnActor(DecalActor, Location, Rotation);
	Destroy();
}

void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

