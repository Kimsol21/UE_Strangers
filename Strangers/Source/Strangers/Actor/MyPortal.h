// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyPortal.generated.h"

UCLASS()
class STRANGERS_API AMyPortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyPortal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override; //Actor 컴포넌트가 완전히 초기화된 이후 호출된다.게임 플레이 중에만 호출

public:	



	//Collision OverlapEvent가 발생했을 때 바인딩할 함수.
	UFUNCTION()
	void OnPlayerCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere, Category = "Entrance")
		class UBoxComponent* PlayerCollisionBox; // 플레이어를 감지할 콜리전.

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		FName TransgerLevelName; //이동할 레벨.

};
