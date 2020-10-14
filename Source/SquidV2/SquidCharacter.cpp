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
	collision = true;
	DistBtwnBuildings = 465.0f;
	side = Right;

	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);
	GetCapsuleComponent()->BodyInstance.SetCollisionProfileName("PhysicsActor");
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ASquidCharacter::OnCompHit);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ASquidCharacter::OnOverlapBegin);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ASquidCharacter::OnOverlapEnd);

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
			Loss();
		}
	}
	if (GetActorLocation().X > 6800.0f) {
		if (GEngine)
		{	
			Win();
		}
	}
	UCharacterMovementComponent* CharMovement = GetCharacterMovement();
	// if (collision) { if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("collided")); }


	
}

// Called to bind functionality to input
void ASquidCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{

	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UCharacterMovementComponent* CharMovement = GetCharacterMovement();
	if (collision) {
		PlayerInputComponent->BindAction("JumpRight", IE_Pressed, this, &ASquidCharacter::StartJumpRight);
		PlayerInputComponent->BindAction("JumpLeft", IE_Pressed, this, &ASquidCharacter::StartJumpLeft);
	}
	PlayerInputComponent->BindAction("JumpRight", IE_Released, this, &ASquidCharacter::StopJump);
	PlayerInputComponent->BindAction("JumpLeft", IE_Released, this, &ASquidCharacter::StopJump);
}

void ASquidCharacter::StartJumpRight()
{
	if (side == Left && collision && GetCharacterMovement()->Velocity.Z == 0.0f) {
		bPressedJump = true;
		UCharacterMovementComponent* CharMovement = GetCharacterMovement();
		CharMovement->Velocity += FVector(0, DistBtwnBuildings, 0);
		rightDir = true;
		side = Right;
		collision = false;
	}
}

void ASquidCharacter::StartJumpLeft()
{
	if (side == Right && collision && GetCharacterMovement()->Velocity.Z == 0.0f) {
		bPressedJump = true;
		UCharacterMovementComponent* CharMovement = GetCharacterMovement();
		CharMovement->Velocity += FVector(0, -DistBtwnBuildings, 0);
		rightDir = false;
		side = Left;
		collision = false;
	}
}

void ASquidCharacter::StopJump()
{
	bPressedJump = false;
}

void ASquidCharacter::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		collision = true;
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("I Hit: "), *OtherActor->GetName()));
		if (OtherActor->GetName().Equals("CubeL3") || OtherComp->GetName().Equals("CubeL3")) {
			Loss();
		}
	}	
}

void ASquidCharacter::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Red, TEXT("TOUCHIN"));
	}
}

void ASquidCharacter::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Red, TEXT("NOT TOUCHIN"));
	}
}

void ASquidCharacter::Loss() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Red, TEXT("YOU LOSE"));
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->ConsoleCommand("quit");
}

void ASquidCharacter::Win() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Green, TEXT("YOU WIN"));
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->ConsoleCommand("quit");
}