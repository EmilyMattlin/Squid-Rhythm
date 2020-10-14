// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "CoreMinimal.h"
#include "SquidCharacter.generated.h"

UENUM()
enum Side {
	Left	UMETA(DisplayName = "Left"),
	Right	UMETA(DisplayName = "Right")
};

UCLASS()
class SQUIDV2_API ASquidCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASquidCharacter();
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* VisualMesh;
	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* MyComp;
	UPROPERTY(VisibleAnywhere)
		bool rightDir;
	UPROPERTY(VisibleAnywhere)
		bool collision;
	UPROPERTY(VisibleAnywhere)
		float DistBtwnBuildings;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Third person camera.
	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* ThirdPCameraComponent;

	// Sets jump flag when key is pressed.
	UFUNCTION()
		void StartJumpRight();
	
	// Sets jump flag when key is pressed.
	UFUNCTION()
		void StartJumpLeft();

	// Clears jump flag when key is released.
	UFUNCTION()
		void StopJump();
	
	// On component hit activated when component collides with other collision component.
	UFUNCTION()
		void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	// Enum for which side the squid is going/on
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Side)
		TEnumAsByte<Side> side;

	/** called when something enters the sphere component */
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** called when something leaves the sphere component */
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	// Win function
	UFUNCTION()
		void Win();

	// Lose function
	UFUNCTION()
		void Loss();
};
