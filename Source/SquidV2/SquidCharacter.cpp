// Fill out your copyright notice in the Description page of Project Settings.
#include "SquidCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Gameframework/CharacterMovementComponent.h"


// Sets default values
ASquidCharacter::ASquidCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	rightDir = true;
	DistBtwnBuildings = 515.0f;
	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);
	GetCapsuleComponent()->BodyInstance.SetCollisionProfileName("BlockAll");
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ASquidCharacter::OnCompHit);
/*
	MyComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	MyComp->SetNotifyRigidBodyCollision(true);

	MyComp->BodyInstance.SetCollisionProfileName("BlockAllDynamic");
	MyComp->OnComponentHit.AddDynamic(this, &ASquidCharacter::OnCompHit);

	// Set as root component
	RootComponent = MyComp;
	*/
	// Make cube
	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	VisualMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube_Character.Shape_Cube_Character"));

	if (CubeVisualAsset.Succeeded())
	{
		VisualMesh->SetStaticMesh(CubeVisualAsset.Object);
		FVector Scale = FVector(1.0f);
		VisualMesh->SetWorldScale3D(Scale * 0.5f);
		VisualMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}
	VisualMesh->SetNotifyRigidBodyCollision(true);
	
	/*MyComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	MyComp->SetSimulatePhysics(true);
	MyComp->SetNotifyRigidBodyCollision(true);

	MyComp->BodyInstance.SetCollisionProfileName("BlockAll");
	MyComp->OnComponentHit.AddDynamic(this, &ASquidCharacter::OnCompHit);
	// Set as root component
	//RootComponent = MyComp;
	MyComp ->SetupAttachment(RootComponent);

	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);

	GetCapsuleComponent->BodyInstance.SetCollisionProfileName("BlockAll");
	GetCapsuleComponent->OnComponentHit.AddDynamic(this, &ASquidCharacter::OnCompHit);
	//RootComponent = VisualMesh;*/

	// Create a first person camera component.
	ThirdPCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	// Attach the camera component to our capsule component.
	ThirdPCameraComponent->SetupAttachment(RootComponent);

	// Position the camerconst FVector* forwardDirectiona slightly above the eyes.
	ThirdPCameraComponent->SetRelativeLocation(FVector(-250.0f, 0.0f, BaseEyeHeight));
	// Allow the pawn to control camera rotation.
	ThirdPCameraComponent->bUsePawnControlRotation = true;

}

// Called when the game starts or when spawned
void ASquidCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (GEngine)
	{
		// Put up a debug message for five seconds. The -1 "Key" value (first argument) indicates that we will never need to update or refresh this message.
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are using SquidCharacter."));
	}
}

// Called every frame
void ASquidCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//Move character forward each tick
	FVector ForwardMovement = GetActorLocation();
	ForwardMovement.X += 3.0f;  
	SetActorLocation(ForwardMovement);

	if (GetActorLocation().Z < 50.0f) {
		if (GEngine)
		{
			// Put up a debug message for five seconds. The -1 "Key" value (first argument) indicates that we will never need to update or refresh this message.
			GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Red, TEXT("YOU LOSE"));
		}
	}
	if (GetActorLocation().X > 6800.0f) {
		if (GEngine)
		{
			// Put up a debug message for five seconds. The -1 "Key" value (first argument) indicates that we will never need to update or refresh this message.
			GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Red, TEXT("YOU WIN"));
		}
	}
}

// Called to bind functionality to input
void ASquidCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up "movement" bindings.
	/*PlayerInputComponent->BindAxis("MoveForward", this, &ASquidCharacter::MoveForward);*/
	//PlayerInputComponent->BindAxis("MoveRight", this, &ASquidCharacter::MoveRight);

	UCharacterMovementComponent* CharMovement = GetCharacterMovement();
	//if (CharMovement->Velocity.Y == 0.0f) {}
	//if (CharMovement->Velocity.Y <= 0.0f) {
		PlayerInputComponent->BindAction("JumpRight", IE_Pressed, this, &ASquidCharacter::StartJumpRight);
	//}
	//if (CharMovement->Velocity.Y >= 0.0f) {
		PlayerInputComponent->BindAction("JumpLeft", IE_Pressed, this, &ASquidCharacter::StartJumpLeft);
	//}
	PlayerInputComponent->BindAction("JumpRight", IE_Released, this, &ASquidCharacter::StopJump);
	PlayerInputComponent->BindAction("JumpLeft", IE_Released, this, &ASquidCharacter::StopJump);
}

void ASquidCharacter::MoveRight(float Value)
{
	// Find out which way is "right" and record that the player wants to move that way.
	//FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	//AddMovementInput(Direction, Value);
	
	FVector NewLocation = GetActorLocation();
	NewLocation.Y += Value*27.0f;       //Add to x distance by 1
	SetActorLocation(NewLocation);
}

void ASquidCharacter::StartJumpRight()
{
	bPressedJump = true;
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Red, TEXT("right"));
	UCharacterMovementComponent* CharMovement = GetCharacterMovement();
	CharMovement->Velocity += FVector(0, DistBtwnBuildings, 0);
	rightDir = true;
}

void ASquidCharacter::StartJumpLeft()
{
	bPressedJump = true;

	UCharacterMovementComponent* CharMovement = GetCharacterMovement();
	CharMovement->Velocity += FVector(0, -DistBtwnBuildings, 0);
	rightDir = false;
}

void ASquidCharacter::StopJump()
{
	bPressedJump = false;
}

void ASquidCharacter::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("I Hit: "), *OtherActor->GetName()));
	}
}