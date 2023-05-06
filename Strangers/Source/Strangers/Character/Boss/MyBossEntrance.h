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
	virtual void PostInitializeComponents() override; //Actor ������Ʈ�� ������ �ʱ�ȭ�� ���� ȣ��ȴ�.���� �÷��� �߿��� ȣ��

	//Collision OverlapEvent�� �߻����� �� ���ε��� �Լ�.
	UFUNCTION()
	void OnPlayerCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/*UFUNCTION()
	void OnTargetCollisionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);*/

	
	UPROPERTY(EditAnywhere, Category = "Entrance")
	AMyBoss* BossToFight; // ������ �ϴ� ����.

private:
	UPROPERTY(VisibleAnywhere, Category = "Entrance")
	UBoxComponent* PlayerCollisionBox; // �÷��̾ ������ �ݸ���.

	

};
