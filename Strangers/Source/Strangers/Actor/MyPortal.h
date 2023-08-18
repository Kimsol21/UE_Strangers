#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyPortal.generated.h"

UCLASS()
class STRANGERS_API AMyPortal : public AActor
{
	GENERATED_BODY()
	
public:	
	AMyPortal();

	UFUNCTION()
	void OnPlayerCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	virtual void PostInitializeComponents() override; 

private:
	UPROPERTY(VisibleAnywhere, Category = "Entrance")
		class UBoxComponent* PlayerCollisionBox; 

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		FName TransgerLevelName; 
};
