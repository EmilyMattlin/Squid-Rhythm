
#pragma once

#include "GameFramework/Actor.h"
#include "OnComponentHit.generated.h"

class UBoxComponent;

UCLASS()
class AOnComponentHit : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AOnComponentHit();

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* MyComp;

	UFUNCTION()
		void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
