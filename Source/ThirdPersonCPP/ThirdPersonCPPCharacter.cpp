// Copyright Epic Games, Inc. All Rights Reserved.

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

#include "ThirdPersonCPPCharacter.h"
#include <DrawDebugHelpers.h>
#include "Engine/StaticMeshActor.h"
#include "ThirdPersonCPPGameMode.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// AThirdPersonCPPCharacter

AThirdPersonCPPCharacter::AThirdPersonCPPCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	//SetLife
	if(Max_Life == 0)
	{
		Max_Life = 100;
	}
	Life = Max_Life;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void AThirdPersonCPPCharacter::ApplyHealthPoint(float Health_point)
{
	if(IsDead)
		return;
	Life += Health_point;
	if(Life <= 0 && !IsDead)
	{
		Die();
	}else if (Life > Max_Life)
	{
		Life = Max_Life;
	}
	
}

void AThirdPersonCPPCharacter::Ragdoll()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName("Ragdoll");
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	FTimerHandle handle;
	GetWorldTimerManager().SetTimer(handle,[this](){Destroy();},5.0f,false);
}

void AThirdPersonCPPCharacter::Die()
{
	IsDead = true;
	Life = 0;
	Ragdoll();
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->AttachToComponent(GetMesh(),FAttachmentTransformRules::KeepWorldTransform,"pelvis");
	FTimerHandle handle;
	GetWorldTimerManager().SetTimer(handle,this,&AThirdPersonCPPCharacter::OnDie,3.0f,false);
}

void AThirdPersonCPPCharacter::OnDie()
{
	AGameModeBase* AuthGameMode = GetWorld()->GetAuthGameMode();
	if(AThirdPersonCPPGameMode* GameMode = Cast<AThirdPersonCPPGameMode>(AuthGameMode) )
	{
		GameMode->OnPlayerKilled(GetController());
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AThirdPersonCPPCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	
	PlayerInputComponent->BindAction("Grab", IE_Pressed, this, &AThirdPersonCPPCharacter::Grab);
	PlayerInputComponent->BindAction("Grab", IE_Released, this, &AThirdPersonCPPCharacter::StopGrab);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AThirdPersonCPPCharacter::Fire);

	PlayerInputComponent->BindAxis("MoveForward", this, &AThirdPersonCPPCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AThirdPersonCPPCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AThirdPersonCPPCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AThirdPersonCPPCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AThirdPersonCPPCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AThirdPersonCPPCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AThirdPersonCPPCharacter::OnResetVR);
}


void AThirdPersonCPPCharacter::OnResetVR()
{
	// If ThirdPersonCPP is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in ThirdPersonCPP.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AThirdPersonCPPCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AThirdPersonCPPCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AThirdPersonCPPCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AThirdPersonCPPCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AThirdPersonCPPCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AThirdPersonCPPCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AThirdPersonCPPCharacter::Grab()
{
	if(IsDead)
		return;
	
	FVector Start = GetCapsuleComponent()->GetComponentLocation();
	FVector ForwardVector = FollowCamera->GetForwardVector();
	FVector End = (ForwardVector * 200) + Start;

	DrawDebugLine(GetWorld(), Start,End, FColor::Red,false,1,0,1);
	
	FHitResult hit;
	if(GetWorld()->LineTraceSingleByChannel(hit,Start,End,ECollisionChannel::ECC_GameTraceChannel1))
	{
		GrabActor = hit.GetActor();
		if(GrabActor)
		{
			UStaticMeshComponent* mesh = Cast<AStaticMeshActor>(GrabActor)->GetStaticMeshComponent();
			mesh->SetSimulatePhysics(false);
			
			GrabActor->SetActorEnableCollision(false);
			GrabActor->AttachToComponent(GetMesh(),FAttachmentTransformRules::KeepRelativeTransform,"LeftHand");
			GrabActor->SetActorLocation(GetMesh()->GetSocketLocation("LeftHand"));
		}
	}
}

void AThirdPersonCPPCharacter::StopGrab()
{
	if(GrabActor)
	{
		GrabActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		
		FTimerHandle handle;
		GetWorldTimerManager().SetTimer(handle, [this](){ GrabActor->SetActorEnableCollision(true); UStaticMeshComponent* mesh = Cast<AStaticMeshActor>(GrabActor)->GetStaticMeshComponent();
		mesh->SetSimulatePhysics(true); },0.1f,false);
	}
}

void AThirdPersonCPPCharacter::Fire(){
	if(IsDead)
		return;
	FVector const* Location =  new FVector(GetMesh()->GetSocketLocation("Head").X,GetMesh()->GetSocketLocation("Head").Y,GetMesh()->GetSocketLocation("Head").Z);
	FRotator const* Rotation =  new FRotator(GetActorRotation());
	GetWorld()->SpawnActor(BulletActor, Location, Rotation);
}
