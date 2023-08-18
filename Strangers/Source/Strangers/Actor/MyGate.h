#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyGate.generated.h"

UCLASS()
class STRANGERS_API AMyGate : public AActor
{
	GENERATED_BODY()
	
public:	
	AMyGate();

	UFUNCTION()
	void OnPlayerCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void OpenGate();
	void CloseGate();
	
protected:
	virtual void PostInitializeComponents() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Entrance")
	class UBoxComponent* PlayerCollisionBox; 

	UPROPERTY(VisibleAnywhere, Category = "Entrance")
	class UBoxComponent* BlockCollisionBox; 

	UPROPERTY(EditAnywhere, Category = "Entrance", meta = (AllowPrivateAccess = "true"))
	uint8 PassableLevel;

	UPROPERTY(EditAnywhere, Category = "Entrance", meta = (AllowPrivateAccess = "true"))
	FString NoticeString;
};
