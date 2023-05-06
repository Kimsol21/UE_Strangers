// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyBossEntrance.generated.h"

class AMyBoss;
class UBoxComponent;



UCLASS()
class STRANGERS_API AMyBossEntrance : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyBossEntrance();

public:
	virtual void PostInitializeComponents() override; //Actor 컴포넌트가 완전히 초기화된 이후 호출된다.게임 플레이 중에만 호출

	//Collision OverlapEvent가 발생했을 때 바인딩할 함수.
	UFUNCTION()
	void OnPlayerCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/*UFUNCTION()
	void OnTargetCollisionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);*/

	
	UPROPERTY(EditAnywhere, Category = "Entrance")
	AMyBoss* BossToFight; // 들어가고자 하는 보스.

private:
	UPROPERTY(VisibleAnywhere, Category = "Entrance")
	UBoxComponent* PlayerCollisionBox; // 플레이어를 감지할 콜리전.

	

};
